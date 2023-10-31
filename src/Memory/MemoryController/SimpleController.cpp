#include "Memory/MemoryController/SimpleController.h"

#include "Logging/Logging.h"
#include "Memory/WaitingCommand.h"

#include <functional>

using namespace NVM::Memory;

using namespace NVM::Memory;
using namespace NVM::Simulation;
using namespace NVM::Logging;

using CommandFunc = std::function<Command*()>;

std::unique_ptr<Command> makeControllerCommand(CommandFunc& func) {
    auto interconnectCommand = func();
    if (!interconnectCommand) return nullptr;

    auto systemCommand = std::unique_ptr<Command>(new WaitingCommand());
    interconnectCommand->setParent(systemCommand.get());
    return std::move(systemCommand);
}

SimpleController::SimpleController() : totalReads(0), totalWrites(0), bankQueues(1) {}

Command* SimpleController::read(uint64_t address, DataBlock data) {
    if (interconnects.empty()) return nullptr;
    if (currentCommand) return nullptr;
    if (bankQueues[0].size() == 10) return nullptr;

    CommandFunc readFunc = [&]() {
        return interconnects[0]->read(address, data);
    };

    currentCommand = std::move(makeControllerCommand(readFunc));
    if (currentCommand) {
        log() << LogLevel::EVENT << "SimpleController received read\n";
        totalReads++;
    }
    return currentCommand.get();
}

Command* SimpleController::write(uint64_t address,
                                 NVM::Simulation::DataBlock data) {
    if (interconnects.empty()) return nullptr;
    if (currentCommand) return nullptr;
    if (bankQueues[0].size() == 10) return nullptr;

    CommandFunc writeFunc = [&]() {
        return interconnects[0]->write(address, data);
    };

    currentCommand = std::move(makeControllerCommand(writeFunc));
    if (currentCommand) {
        log() << LogLevel::EVENT << "SimpleController received write\n";
        totalWrites++;
    }
    return currentCommand.get();
}

Command* SimpleController::rowClone(uint64_t srcAddress, uint64_t destAddress,
                                    NVM::Simulation::DataBlock data) {
    if (interconnects.empty()) return nullptr;
    if (currentCommand) return nullptr;

    CommandFunc rowCloneFunc = [&]() {
        return interconnects[0]->rowClone(srcAddress, destAddress, data);
    };

    currentCommand = std::move(makeControllerCommand(rowCloneFunc));
    if (currentCommand) {
        log() << LogLevel::EVENT << "SimpleController received row clone\n";
    }
    return currentCommand.get();
}

Command*
SimpleController::transverseRead(uint64_t baseAddress, uint64_t destAddress,
                                 std::vector<NVM::Simulation::DataBlock> data) {
    if (interconnects.empty()) return nullptr;
    if (currentCommand) return nullptr;

    CommandFunc writeFunc = [&]() {
        return interconnects[0]->transverseRead(baseAddress, destAddress, data);
    };

    currentCommand = std::move(makeControllerCommand(writeFunc));
    if (currentCommand) {
        log() << LogLevel::EVENT
              << "SimpleController received transverse read\n";
    }
    return currentCommand.get();
}

Command* SimpleController::transverseWrite(
    uint64_t address, std::vector<NVM::Simulation::DataBlock> data) {
    if (interconnects.empty()) return nullptr;
    if (currentCommand) return nullptr;

    CommandFunc writeFunc = [&]() {
        return interconnects[0]->transverseWrite(address, data);
    };

    currentCommand = std::move(makeControllerCommand(writeFunc));
    if (currentCommand) {
        log() << LogLevel::EVENT
              << "SimpleController received transverse write\n";
    }
    return currentCommand.get();
}

void SimpleController::cycle(unsigned int cycles) {
    if (!interconnects.empty()) interconnects[0]->cycle(cycles);
    if (!currentCommand) return;
    if (static_cast<WaitingCommand*>(currentCommand.get())->isDone())
        currentCommand.reset();
}

bool SimpleController::isEmpty() const { return currentCommand == nullptr; }

void SimpleController::addInterconnect(
    std::unique_ptr<Interconnect> interconnect) {
    interconnects.emplace_back(std::move(interconnect));
}

StatBlock SimpleController::getStats(std::string tag) const {
    StatBlock stats(tag);

    stats.addStat(&totalReads, "reads");
    stats.addStat(&totalWrites, "writes");

    for (int i = 0; i < interconnects.size(); i++) {
        stats.addChild(
            interconnects[i]->getStats(tag + ".i" + std::to_string(i)));
    }

    return stats;
}