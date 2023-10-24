#include "Memory/SubArray/SimpleSubArray.h"

#include "Logging/Logging.h"
#include "Memory/TimedCommand.h"

using namespace NVM::Memory;
using namespace NVM::Logging;

SimpleSubArray::SimpleSubArray() :
    totalReads(0),
    totalWrites(0),
    currentCommand(nullptr) {}

Command* SimpleSubArray::read(uint64_t address,
                              NVM::Simulation::DataBlock data) {
    if (currentCommand) return nullptr;
    currentCommand = std::unique_ptr<Command>(new TimedCommand());
    if (currentCommand) {
        log() << LogLevel::EVENT << "SubArray received read\n";
        totalReads++;
    }
    return currentCommand.get();
}

Command* SimpleSubArray::write(uint64_t address,
                               NVM::Simulation::DataBlock data) {
    if (currentCommand) return nullptr;
    currentCommand = std::unique_ptr<Command>(new TimedCommand());
    if (currentCommand) {
        log() << LogLevel::EVENT << "SubArray received write\n";
        totalWrites++;
    }
    return currentCommand.get();
}

void SimpleSubArray::cycle(unsigned int cycles) {
    if (!currentCommand) return;
    TimedCommand* cmd = static_cast<TimedCommand*>(currentCommand.get());
    cmd->cycle(cycles);
    if (cmd->isDone()) currentCommand.reset();
}

bool SimpleSubArray::isEmpty() const { return currentCommand == nullptr; }

StatBlock SimpleSubArray::getStats(std::string tag) const {
    StatBlock stats(tag);

    stats.addStat(&totalReads, "reads");
    stats.addStat(&totalWrites, "writes");

    return stats;
}