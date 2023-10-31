#include "Memory/MemorySystem/SimpleSystem.h"

#include "Logging/Logging.h"
#include "Memory/Command/WaitingCommand.h"

#include <functional>

using namespace NVM::Memory;

using namespace NVM::Memory;
using namespace NVM::Simulation;
using namespace NVM::Logging;

bool SimpleSystem::issue(NVMainRequest* req) { return false; }

using CommandFunc = std::function<Command*()>;

std::unique_ptr<Command> makeSystemCommand(CommandFunc& func) {
    auto interconnectCommand = func();
    if (!interconnectCommand) return nullptr;

    auto systemCommand = std::unique_ptr<Command>(new WaitingCommand());
    interconnectCommand->setParent(systemCommand.get());
    return std::move(systemCommand);
}

SimpleSystem::SimpleSystem() : totalReads(0), totalWrites(0), currentCycle(0) {}

bool SimpleSystem::read(uint64_t address, DataBlock data, unsigned int threadId,
                        unsigned int cycle) {
    if (channels.empty()) return false;
    if (currentCommand) return false;

    CommandFunc readFunc = [&]() { return channels[0]->read(address, data); };

    currentCommand = std::move(makeSystemCommand(readFunc));
    if (currentCommand) {
        totalReads++;
        log() << LogLevel::EVENT << "SimpleSystem received read\n";
    }
    return currentCommand != nullptr;
}

bool SimpleSystem::write(uint64_t address, NVM::Simulation::DataBlock data,
                         unsigned int threadId, unsigned int cycle) {
    if (channels.empty()) return false;
    if (currentCommand) return false;

    CommandFunc writeFunc = [&]() { return channels[0]->write(address, data); };

    currentCommand = std::move(makeSystemCommand(writeFunc));
    if (currentCommand) {
        log() << LogLevel::EVENT << "SimpleSystem received write\n";
        totalWrites++;
    }
    return currentCommand != nullptr;
}

bool SimpleSystem::rowClone(uint64_t srcAddress, uint64_t destAddress,
                            NVM::Simulation::DataBlock data,
                            unsigned int threadId, unsigned int cycle) {
    if (channels.empty()) return false;
    if (currentCommand) return false;

    CommandFunc rowCloneFunc = [&]() {
        return channels[0]->rowClone(srcAddress, destAddress, data);
    };

    currentCommand = std::move(makeSystemCommand(rowCloneFunc));
    if (currentCommand) {
        log() << LogLevel::EVENT << "SimpleSystem received row clone\n";
    }
    return currentCommand != nullptr;
}

bool SimpleSystem::transverseRead(
    uint64_t baseAddress, uint64_t destAddress,
    std::vector<NVM::Simulation::DataBlock> inputRows, unsigned int threadId,
    unsigned int cycle) {
    if (channels.empty()) return false;
    if (currentCommand) return false;

    CommandFunc trFunc = [&]() {
        return channels[0]->transverseRead(baseAddress, destAddress, inputRows);
    };

    currentCommand = std::move(makeSystemCommand(trFunc));
    if (currentCommand) {
        log() << LogLevel::EVENT << "SimpleSystem received transverse read\n";
    }
    return currentCommand != nullptr;
}

bool SimpleSystem::transverseWrite(
    uint64_t baseAddress, std::vector<NVM::Simulation::DataBlock> writeData,
    unsigned int threadId, unsigned int cycle) {
    if (channels.empty()) return false;
    if (currentCommand) return false;

    CommandFunc twFunc = [&]() {
        return channels[0]->transverseWrite(baseAddress, writeData);
    };

    currentCommand = std::move(makeSystemCommand(twFunc));
    if (currentCommand) {
        log() << LogLevel::EVENT << "SimpleSystem received transverse write\n";
    }
    return currentCommand != nullptr;
}

bool SimpleSystem::shift(uint64_t address, unsigned int shiftAmount,
                         NVM::Simulation::DataBlock data, unsigned int threadId,
                         unsigned int cycle) {
    return true;
}

void SimpleSystem::cycle(unsigned int cycles) {
    if (!channels.empty()) channels[0]->cycle(cycles);
    currentCycle += cycles;
    if (!currentCommand) return;
    if (static_cast<WaitingCommand*>(currentCommand.get())->isDone())
        currentCommand.reset();
}

bool SimpleSystem::isEmpty() const { return currentCommand == nullptr; }

void SimpleSystem::addController(
    std::unique_ptr<MemoryController> interconnect) {
    channels.emplace_back(std::move(interconnect));
}

void SimpleSystem::printStats(std::ostream& statStream) {
    StatBlock stats("system");

    stats.addStat(&totalReads, "reads");
    stats.addStat(&totalWrites, "writes");

    for (int i = 0; i < channels.size(); i++) {
        stats.addChild(
            channels[i]->getStats("system.channel" + std::to_string(i)));
    }

    stats.log(statStream);
}

unsigned int SimpleSystem::getCurrentCycle() { return currentCycle; }