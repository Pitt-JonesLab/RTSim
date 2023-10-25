#include "Memory/SubArray/SimpleSubArray.h"

#include "Logging/Logging.h"
#include "Memory/ChainedCommand.h"
#include "Memory/NullCommand.h"
#include "Memory/TimedCommand.h"

#include <functional>

using namespace NVM::Memory;
using namespace NVM::Logging;

SimpleSubArray::SimpleSubArray(unsigned int rows) :
    totalReads(0),
    totalWrites(0),
    totalActivates(0),
    totalPrecharges(0),
    rowBufferHits(0),
    currentCommand(nullptr),
    rowControl(rows, {1, 1}) {}

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
    stats.addStat(&totalActivates, "activates");
    stats.addStat(&totalPrecharges, "precharges");
    stats.addStat(&rowBufferHits, "row_buffer_hits");

    return stats;
}

Command* SimpleSubArray::switchRow(unsigned int row) {
    if (currentCommand) return nullptr;

    if (rowControl.rowIsOpen(row)) {
        rowBufferHits++;
        log() << LogLevel::EVENT << "Row " << row << " is already activated\n";
        return sendNull();
    }

    // TODO: RowController should handle these stats
    totalActivates++;
    totalPrecharges++;
    std::vector<std::function<Command*()>> switchCmds = {
        [this]() { return rowControl.closeRow(); },
        [this,row]() { return rowControl.activate(row); }};

    currentCommand.reset(new ChainedCommand(switchCmds));
    return currentCommand.get();
}