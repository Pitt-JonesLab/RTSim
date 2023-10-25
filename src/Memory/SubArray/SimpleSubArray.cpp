#include "Memory/SubArray/SimpleSubArray.h"

#include "Logging/Logging.h"
#include "Memory/ChainedCommand.h"
#include "Memory/NullCommand.h"
#include "Memory/TimedCommand.h"

#include <functional>

using namespace NVM::Memory;
using namespace NVM::Logging;

SimpleSubArray::SimpleSubArray() :
    totalReads(0),
    totalWrites(0),
    rowBufferHits(0),
    currentCommand(nullptr),
    rowControl(10, {1, 1}) {}

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
    currentCommand->cycle(cycles);
    rowControl.cycle(cycles);
    if (currentCommand->isDone()) currentCommand.reset();
}

bool SimpleSubArray::isEmpty() const { return currentCommand == nullptr; }

StatBlock SimpleSubArray::getStats(std::string tag) const {
    StatBlock stats(tag);

    stats.addStat(&totalReads, "reads");
    stats.addStat(&totalWrites, "writes");
    stats.addStat(&rowBufferHits, "row_buffer_hits");

    return stats;
}

Command* SimpleSubArray::switchRow(unsigned int row) {
    if (currentCommand) return nullptr;
    if (rowControl.rowIsOpen(row)) {
        rowBufferHits++;
        return sendNull();
    }

    std::vector<std::function<Command*()>> switchCmds = {
        [&]() { return rowControl.closeRow(); },
        [&]() { return rowControl.activate(row); }};

    currentCommand.reset(new ChainedCommand(switchCmds));
    return currentCommand.get();
}