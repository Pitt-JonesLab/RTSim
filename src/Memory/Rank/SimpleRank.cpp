#include "Memory/Rank/SimpleRank.h"

#include "Logging/Logging.h"
#include "Memory/WaitingCommand.h"

#include <functional>

using namespace NVM::Memory;

using namespace NVM::Memory;
using namespace NVM::Simulation;
using namespace NVM::Logging;

using CommandFunc = std::function<Command*()>;

std::unique_ptr<Command> makeRankCommand(CommandFunc& func) {
    auto bankCommand = func();
    if (!bankCommand) return nullptr;

    auto rankCommand = std::unique_ptr<Command>(new WaitingCommand());
    bankCommand->setParent(rankCommand.get());
    return std::move(rankCommand);
}

SimpleRank::SimpleRank() : totalReads(0), totalWrites(0) {}

Command* SimpleRank::read(uint64_t address, DataBlock data) {
    if (banks.empty()) return nullptr;
    if (currentCommand) return nullptr;

    CommandFunc readFunc = [&]() { return banks[0]->read(address, data); };

    currentCommand = std::move(makeRankCommand(readFunc));
    if (currentCommand) {
        log() << LogLevel::EVENT << "SimpleRank received read\n";
        totalReads++;
    }
    return currentCommand.get();
}

Command* SimpleRank::write(uint64_t address, NVM::Simulation::DataBlock data) {
    if (banks.empty()) return nullptr;
    if (currentCommand) return nullptr;

    CommandFunc writeFunc = [&]() { return banks[0]->write(address, data); };

    currentCommand = std::move(makeRankCommand(writeFunc));
    if (currentCommand) {
        log() << LogLevel::EVENT << "SimpleRank received read\n";
        totalWrites++;
    }
    return currentCommand.get();
}

Command* SimpleRank::rowClone(uint64_t srcAddress, uint64_t destAddress,
                              NVM::Simulation::DataBlock data) {
    if (banks.empty()) return nullptr;
    if (currentCommand) return nullptr;

    CommandFunc writeFunc = [&]() {
        return banks[0]->rowClone(srcAddress, destAddress, data);
    };

    currentCommand = std::move(makeRankCommand(writeFunc));
    if (currentCommand) {
        log() << LogLevel::EVENT << "SimpleRank received row clone\n";
    }
    return currentCommand.get();
}

Command*
SimpleRank::transverseRead(uint64_t baseAddress, uint64_t destAddress,
                           std::vector<NVM::Simulation::DataBlock> data) {
    if (banks.empty()) return nullptr;
    if (currentCommand) return nullptr;

    CommandFunc writeFunc = [&]() {
        return banks[0]->transverseRead(baseAddress, destAddress, data);
    };

    currentCommand = std::move(makeRankCommand(writeFunc));
    if (currentCommand) {
        log() << LogLevel::EVENT << "SimpleRank received transverse read\n";
    }
    return currentCommand.get();
}

Command*
SimpleRank::transverseWrite(uint64_t address,
                            std::vector<NVM::Simulation::DataBlock> data) {
    if (banks.empty()) return nullptr;
    if (currentCommand) return nullptr;

    CommandFunc writeFunc = [&]() {
        return banks[0]->transverseWrite(address, data);
    };

    currentCommand = std::move(makeRankCommand(writeFunc));
    if (currentCommand) {
        log() << LogLevel::EVENT << "SimpleRank received transverse write\n";
    }
    return currentCommand.get();
}

void SimpleRank::cycle(unsigned int cycles) {
    if (!banks.empty()) banks[0]->cycle(cycles);
    if (!currentCommand) return;
    if (static_cast<WaitingCommand*>(currentCommand.get())->isDone())
        currentCommand.reset();
}

bool SimpleRank::isEmpty() const { return currentCommand == nullptr; }

void SimpleRank::addBank(std::unique_ptr<Bank> bank) {
    banks.emplace_back(std::move(bank));
}

StatBlock SimpleRank::getStats(std::string tag) const {
    StatBlock stats(tag);

    stats.addStat(&totalReads, "reads");
    stats.addStat(&totalWrites, "writes");

    for (int i = 0; i < banks.size(); i++) {
        auto bankStats = banks[i]->getStats(tag + ".bank" + std::to_string(i));
        stats.addChildStat(bankStats, "activate_energy", "nJ");
        stats.addChildStat(bankStats, "read_energy", "nJ");
        stats.addChildStat(bankStats, "write_energy", "nJ");
        stats.addChildStat(bankStats, "reads");
        stats.addChildStat(bankStats, "writes");
        stats.addChildStat(bankStats, "activates");
        stats.addChildStat(bankStats, "precharges");
        stats.addChildStat(bankStats, "row_clones");
        stats.addChildStat(bankStats, "transverse_reads");
        stats.addChildStat(bankStats, "transverse_writes");
        stats.addChild(bankStats);
    }

    return stats;
}