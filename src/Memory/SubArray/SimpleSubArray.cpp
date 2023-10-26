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
    actEnergy(0),
    writeEnergy(0),
    readEnergy(0),
    totalShifts(0),
    shiftEnergy(0),
    currentCommand(nullptr),
    rowControl(rows, {1, 1}) {}

Command* SimpleSubArray::read(uint64_t address,
                              NVM::Simulation::DataBlock data) {
    if (currentCommand) return nullptr;
    currentCommand = std::unique_ptr<Command>(new TimedCommand());
    if (currentCommand) {
        log() << LogLevel::EVENT << "SubArray received read\n";
        totalReads++;
        totalShifts++;
        readEnergy += 0.11;
        shiftEnergy += 0.03;
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
        totalShifts;
        writeEnergy += 18.3;
        shiftEnergy += 0.03;
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
    stats.addStat(&totalShifts, "shifts");
    stats.addStat(&rowBufferHits, "row_buffer_hits");
    stats.addStat(&actEnergy, "activate_energy");
    stats.addStat(&readEnergy, "read_energy");
    stats.addStat(&writeEnergy, "write_energy");
    stats.addStat(&shiftEnergy, "shift_energy");

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
    actEnergy += 0.080096;
    std::vector<std::function<Command*()>> switchCmds = {
        [this]() { return rowControl.closeRow(); },
        [this, row]() { return rowControl.activate(row); }};

    currentCommand.reset(new ChainedCommand(switchCmds));
    return currentCommand.get();
}