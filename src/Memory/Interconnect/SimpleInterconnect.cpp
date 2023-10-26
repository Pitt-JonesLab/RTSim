#include "Memory/Interconnect/SimpleInterconnect.h"

#include "Logging/Logging.h"
#include "Memory/WaitingCommand.h"

#include <functional>

using namespace NVM::Memory;

using namespace NVM::Memory;
using namespace NVM::Simulation;
using namespace NVM::Logging;

using CommandFunc = std::function<Command*()>;

std::unique_ptr<Command> makeInterconnectCommand(CommandFunc& func) {
    auto rankCommand = func();
    if (!rankCommand) return nullptr;

    auto interCommand = std::unique_ptr<Command>(new WaitingCommand());
    rankCommand->setParent(interCommand.get());
    return std::move(interCommand);
}

SimpleInterconnect::SimpleInterconnect() : totalReads(0), totalWrites(0) {}

Command* SimpleInterconnect::read(uint64_t address, DataBlock data) {
    if (ranks.empty()) return nullptr;
    if (currentCommand) return nullptr;

    CommandFunc readFunc = [&]() { return ranks[0]->read(address, data); };

    currentCommand = std::move(makeInterconnectCommand(readFunc));
    if (currentCommand) {
        log() << LogLevel::EVENT << "SimpleInterconnect received read\n";
        totalReads++;
    }
    return currentCommand.get();
}

Command* SimpleInterconnect::write(uint64_t address,
                                   NVM::Simulation::DataBlock data) {
    if (ranks.empty()) return nullptr;
    if (currentCommand) return nullptr;

    CommandFunc writeFunc = [&]() { return ranks[0]->write(address, data); };

    currentCommand = std::move(makeInterconnectCommand(writeFunc));
    if (currentCommand) {
        log() << LogLevel::EVENT << "SimpleInterconnect received read\n";
        totalWrites++;
    }
    return currentCommand.get();
}

Command* SimpleInterconnect::rowClone(uint64_t srcAddress, uint64_t destAddress,
                                      NVM::Simulation::DataBlock data) {
    if (ranks.empty()) return nullptr;
    if (currentCommand) return nullptr;

    CommandFunc writeFunc = [&]() {
        return ranks[0]->rowClone(srcAddress, destAddress, data);
    };

    currentCommand = std::move(makeInterconnectCommand(writeFunc));
    if (currentCommand) {
        log() << LogLevel::EVENT << "SimpleInterconnect received row clone\n";
        totalWrites++;
    }
    return currentCommand.get();
}

Command* SimpleInterconnect::transverseRead(
    uint64_t baseAddress, uint64_t destAddress,
    std::vector<NVM::Simulation::DataBlock> data) {
    if (ranks.empty()) return nullptr;
    if (currentCommand) return nullptr;

    CommandFunc writeFunc = [&]() {
        return ranks[0]->transverseRead(baseAddress, destAddress, data);
    };

    currentCommand = std::move(makeInterconnectCommand(writeFunc));
    if (currentCommand) {
        log() << LogLevel::EVENT
              << "SimpleInterconnect received transverse read\n";
        totalWrites++;
    }
    return currentCommand.get();
}

Command* SimpleInterconnect::transverseWrite(
    uint64_t address, std::vector<NVM::Simulation::DataBlock> data) {
    if (ranks.empty()) return nullptr;
    if (currentCommand) return nullptr;

    CommandFunc writeFunc = [&]() {
        return ranks[0]->transverseWrite(address, data);
    };

    currentCommand = std::move(makeInterconnectCommand(writeFunc));
    if (currentCommand) {
        log() << LogLevel::EVENT
              << "SimpleInterconnect received transverse write\n";
        totalWrites++;
    }
    return currentCommand.get();
}

void SimpleInterconnect::cycle(unsigned int cycles) {
    if (!ranks.empty()) ranks[0]->cycle(cycles);
    if (!currentCommand) return;
    if (static_cast<WaitingCommand*>(currentCommand.get())->isDone())
        currentCommand.reset();
}

bool SimpleInterconnect::isEmpty() const { return currentCommand == nullptr; }

void SimpleInterconnect::addRank(std::unique_ptr<Rank> rank) {
    ranks.emplace_back(std::move(rank));
}

StatBlock SimpleInterconnect::getStats(std::string tag) const {
    StatBlock stats(tag);

    stats.addStat(&totalReads, "reads");
    stats.addStat(&totalWrites, "writes");

    for (int i = 0; i < ranks.size(); i++) {
        stats.addChild(ranks[i]->getStats(tag + ".rank" + std::to_string(i)));
    }

    return stats;
}