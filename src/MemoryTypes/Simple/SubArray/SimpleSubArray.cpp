#include "MemoryTypes/Simple/SubArray/SimpleSubArray.h"

#include "Command.h"
#include "Logging/Logging.h"

using namespace NVM::Memory;
using namespace NVM::Logging;

SimpleSubArray::SimpleSubArray(unsigned int rows,
                               std::unique_ptr<CommandTimer> timer,
                               FaultModel faultModel, unsigned int numTries) :
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
    numFaults(0),
    numUncorrectableFaults(0),
    pimEnergy(0),
    faultModel(faultModel),
    numTries(numTries),
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
    stats.addStat(&actEnergy, "activate_energy", "nJ");
    stats.addStat(&readEnergy, "read_energy", "nJ");
    stats.addStat(&writeEnergy, "write_energy", "nJ");
    stats.addStat(&shiftEnergy, "shift_energy", "nJ");
    stats.addStat(&pimEnergy, "pim_energy", "nJ");
    stats.addStat(&TWEnergy, "transverse_write_energy", "nJ");
    stats.addStat(&totalEnergy, "total_energy", "nJ");
    stats.addStat(&numFaults, "pim_faults");
    stats.addStat(&numUncorrectableFaults, "uncorrectable_faults");

    return stats;
}

void SimpleSubArray::checkFaults() {
    for (int i = 0; i < numTries; i++) {
        if (faultModel.check()) {
            timer->addFaultDelay({CommandType::PIM, 0});
            totalTRs++;
            numFaults++;
            pimEnergy += 0.256;
            log() << LogLevel::EVENT << "PIM Fault!\n";
        }
    }
    numUncorrectableFaults = faultModel.getUncorrectableFaults();
}

bool SimpleSubArray::issue(NVM::Command cmd) {
    if (timer->busy()) return false;
    timer->issue(cmd, numTries);

    switch (cmd.getType()) {
        case CommandType::READ:
            totalReads += numTries;
            totalShifts++;
            readEnergy += 0.11 * numTries;
            shiftEnergy += 0.03;
            break;
        case CommandType::WRITE:
            totalWrites += numTries;
            totalShifts++;
            writeEnergy += 18.3 * numTries;
            shiftEnergy += 0.03;
            break;
        case CommandType::ROWCLONE:
            totalRCs += numTries;
            totalActivates++;
            break;
        case CommandType::PIM:
            totalTRs += numTries;
            pimEnergy += 0.256 * numTries;
            checkFaults();
            break;
        case CommandType::ACTIVATE:
            totalActivates++;
            break;
        case CommandType::PRECHARGE:
            totalPrecharges++;
            break;
        case CommandType::TRANSVERSE_WRITE:
            totalTWs++;
            TWEnergy += 0.403;
            break;
    }
    totalEnergy = readEnergy + writeEnergy + actEnergy + shiftEnergy +
                  pimEnergy + TWEnergy;

    return true;
}