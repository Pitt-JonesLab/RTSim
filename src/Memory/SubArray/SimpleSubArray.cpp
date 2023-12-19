#include "Memory/SubArray/SimpleSubArray.h"

#include "Command.h"

using namespace NVM::Memory;

SimpleSubArray::SimpleSubArray(unsigned int rows,
                               std::unique_ptr<CommandTimer> timer) :
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
    totalRCs(0),
    totalTRs(0),
    totalTWs(0),
    timer(std::move(timer)) {}

void SimpleSubArray::cycle(unsigned int cycles) { timer->cycle(); }

bool SimpleSubArray::isEmpty() const { return !timer->busy(); }

NVM::Stats::StatBlock SimpleSubArray::getStats(std::string tag) const {
    NVM::Stats::StatBlock stats(tag);

    stats.addStat(&totalReads, "reads");
    stats.addStat(&totalWrites, "writes");
    stats.addStat(&totalActivates, "activates");
    stats.addStat(&totalPrecharges, "precharges");
    stats.addStat(&totalShifts, "shifts");
    stats.addStat(&totalRCs, "row_clones");
    stats.addStat(&totalTRs, "transverse_reads");
    stats.addStat(&totalTWs, "transverse_writes");
    stats.addStat(&rowBufferHits, "row_buffer_hits");
    stats.addStat(&actEnergy, "activate_energy");
    stats.addStat(&readEnergy, "read_energy");
    stats.addStat(&writeEnergy, "write_energy");
    stats.addStat(&shiftEnergy, "shift_energy");

    return stats;
}

bool SimpleSubArray::issue(NVM::Command cmd) {
    if (timer->busy()) return false;
    timer->issue(cmd);

    switch (cmd.getType()) {
        case CommandType::READ:
            totalReads++;
            totalShifts++;
            readEnergy += 0.11;
            shiftEnergy += 0.03;
            break;
        case CommandType::WRITE:
            totalWrites++;
            totalShifts++;
            writeEnergy += 18.3;
            shiftEnergy += 0.03;
            break;
        case CommandType::ROWCLONE:
            totalRCs++;
            totalActivates++;
            break;
        case CommandType::PIM:
            totalTRs++;
            actEnergy += 0.080096;
            break;
        case CommandType::ACTIVATE:
            totalActivates++;
            break;
        case CommandType::PRECHARGE:
            totalPrecharges++;
            break;
    }

    return true;
}