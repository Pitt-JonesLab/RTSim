#include "Memory/Bank/SimpleBank.h"

#include "Logging/Logging.h"
#include "Memory/ChainedCommand.h"
#include "Memory/Decoder.h"
#include "Memory/WaitingCommand.h"

#include <functional>

using namespace NVM::Memory;
using namespace NVM::Simulation;
using namespace NVM::Logging;

std::unique_ptr<Command> SimpleBank::makeBankCommand(CommandFunc& func,
                                                     uint64_t address) {
    auto row = Decoder::decodeSymbol(Decoder::AddressSymbol::ROW, address);
    std::vector<CommandFunc> bankCmds = {
        [this, row]() { return subArrays[0]->switchRow(row); }, func};

    auto subCmd = std::unique_ptr<Command>(new ChainedCommand(bankCmds));

    return std::move(subCmd);
}

SimpleBank::SimpleBank() : totalReads(0), totalWrites(0) {}

Command* SimpleBank::read(uint64_t address, DataBlock data) {
    if (subArrays.empty()) return nullptr;
    if (currentCommand) return nullptr;

    CommandFunc readFunc = [&]() { return subArrays[0]->read(address, data); };

    currentCommand = std::move(makeBankCommand(readFunc, address));
    if (currentCommand) {
        log() << LogLevel::EVENT << "SimpleBank received read\n";
        totalReads++;
    }
    return currentCommand.get();
}

Command* SimpleBank::write(uint64_t address, NVM::Simulation::DataBlock data) {
    if (subArrays.empty()) return nullptr;
    if (currentCommand) return nullptr;

    CommandFunc writeFunc = [&]() {
        return subArrays[0]->write(address, data);
    };

    currentCommand = std::move(makeBankCommand(writeFunc, address));
    if (currentCommand) {
        log() << LogLevel::EVENT << "SimpleBank received write\n";
    }
    return currentCommand.get();
}

Command* SimpleBank::rowClone(uint64_t srcAddress, uint64_t destAddress,
                              NVM::Simulation::DataBlock data) {
    if (subArrays.empty()) return nullptr;
    if (currentCommand) return nullptr;

    CommandFunc writeFunc = [&]() {
        return subArrays[0]->rowClone(srcAddress, destAddress, data);
    };

    currentCommand = std::move(makeBankCommand(writeFunc, srcAddress));
    if (currentCommand) {
        log() << LogLevel::EVENT << "SimpleBank received row clone\n";
    }
    return currentCommand.get();
}

Command*
SimpleBank::transverseRead(uint64_t baseAddress, uint64_t destAddress,
                           std::vector<NVM::Simulation::DataBlock> data) {
    if (subArrays.empty()) return nullptr;
    if (currentCommand) return nullptr;

    CommandFunc writeFunc = [this, baseAddress, destAddress, data]() {
        return subArrays[0]->transverseRead(baseAddress, destAddress, data);
    };

    currentCommand = std::move(makeBankCommand(writeFunc, baseAddress));
    if (currentCommand) {
        log() << LogLevel::EVENT << "SimpleBank received transverse read\n";
    }
    return currentCommand.get();
}

Command*
SimpleBank::transverseWrite(uint64_t address,
                            std::vector<NVM::Simulation::DataBlock> data) {
    if (subArrays.empty()) return nullptr;
    if (currentCommand) return nullptr;

    CommandFunc writeFunc = [this, address, data]() {
        return subArrays[0]->transverseWrite(address, data);
    };

    currentCommand = std::move(makeBankCommand(writeFunc, address));
    if (currentCommand) {
        log() << LogLevel::EVENT << "SimpleBank received transverse write\n";
        totalWrites++;
    }
    return currentCommand.get();
}

void SimpleBank::cycle(unsigned int cycles) {
    if (!subArrays.empty()) subArrays[0]->cycle(cycles);
    if (!currentCommand) return;
    currentCommand->cycle(cycles);
    if (static_cast<WaitingCommand*>(currentCommand.get())->isDone())
        currentCommand.reset();
}

bool SimpleBank::isEmpty() const { return currentCommand == nullptr; }

void SimpleBank::addSubArray(std::unique_ptr<SubArray> subArray) {
    subArrays.emplace_back(std::move(subArray));
}

StatBlock SimpleBank::getStats(std::string tag) const {
    StatBlock stats(tag);

    //stats.addStat(&totalReads, "reads");
    //stats.addStat(&totalWrites, "writes");

    for (int i = 0; i < subArrays.size(); i++) {
        auto subArrayStats =
            subArrays[i]->getStats(tag + ".subArray" + std::to_string(i));
        stats.addChildStat(subArrayStats, "activate_energy", "nJ");
        stats.addChildStat(subArrayStats, "read_energy", "nJ");
        stats.addChildStat(subArrayStats, "write_energy", "nJ");
        stats.addChildStat(subArrayStats, "reads");
        stats.addChildStat(subArrayStats, "writes");
        stats.addChildStat(subArrayStats, "activates");
        stats.addChildStat(subArrayStats, "precharges");
        stats.addChildStat(subArrayStats, "row_clones");
        stats.addChildStat(subArrayStats, "transverse_reads");
        stats.addChildStat(subArrayStats, "transverse_writes");
        stats.addChild(subArrayStats);
    }

    return stats;
}