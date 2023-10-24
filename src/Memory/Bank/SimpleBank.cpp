#include "Memory/Bank/SimpleBank.h"

#include "Logging/Logging.h"
#include "Memory/WaitingCommand.h"
#include "Memory/ChainedCommand.h"

#include <functional>

using namespace NVM::Memory;
using namespace NVM::Simulation;
using namespace NVM::Logging;

std::unique_ptr<Command> SimpleBank::makeBankCommand(CommandFunc& func) {
    std::vector<CommandFunc> bankCmds = 
        {[&]() { return subArrays[0]->switchRow(0); }, func};

    auto subCmd = std::unique_ptr<Command>
        (new ChainedCommand(bankCmds));

    return std::move(subCmd);
}

SimpleBank::SimpleBank() : totalReads(0), totalWrites(0) {}

Command* SimpleBank::read(uint64_t address, DataBlock data) {
    if (subArrays.empty()) return nullptr;
    if (currentCommand) return nullptr;

    CommandFunc readFunc = [&]() { return subArrays[0]->read(address, data); };

    currentCommand = std::move(makeBankCommand(readFunc));
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

    currentCommand = std::move(makeBankCommand(writeFunc));
    if (currentCommand) {
        log() << LogLevel::EVENT << "SimpleBank received write\n";
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

    stats.addStat(&totalReads, "reads");
    stats.addStat(&totalWrites, "writes");

    for (int i = 0; i < subArrays.size(); i++) {
        stats.addChild(
            subArrays[i]->getStats(tag + ".subArray" + std::to_string(i)));
    }

    return stats;
}