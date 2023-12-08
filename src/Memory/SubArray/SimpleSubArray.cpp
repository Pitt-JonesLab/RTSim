#include "Memory/SubArray/SimpleSubArray.h"

#include "Command.h"
#include "Logging/Logging.h"
#include "Memory/Command/ChainedCommand.h"
#include "Memory/Command/NullCommand.h"
#include "Memory/Command/TimedCommand.h"

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
    totalRCs(0),
    totalTRs(0),
    totalTWs(0) {}

Command* SimpleSubArray::read(uint64_t address,
                              NVM::Simulation::DataBlock data) {
    if (!timer.isAvailable(address)) return nullptr;
    timer.issue({NVM::CommandType::READ, address});
    log() << LogLevel::EVENT << "SubArray received read\n";
    totalReads++;
    totalShifts++;
    readEnergy += 0.11;
    shiftEnergy += 0.03;
    return (Command*) 1;
}

Command* SimpleSubArray::write(uint64_t address,
                               NVM::Simulation::DataBlock data) {
    if (!timer.isAvailable(address)) return nullptr;
    timer.issue({NVM::CommandType::WRITE, address});
    log() << LogLevel::EVENT << "SubArray received write\n";
    totalWrites++;
    totalShifts++;
    writeEnergy += 18.3;
    shiftEnergy += 0.03;
    return (Command*) 1;
}

Command* SimpleSubArray::rowClone(uint64_t srcAddress, uint64_t destAddress,
                                  NVM::Simulation::DataBlock data) {
    if (!timer.isAvailable(srcAddress)) return nullptr;
    timer.issue({NVM::CommandType::ROWCLONE, srcAddress});
    log() << LogLevel::EVENT << "SubArray received row clone\n";
    totalRCs++;
    totalActivates++;
    return (Command*) 1;
}

Command*
SimpleSubArray::transverseRead(uint64_t baseAddress, uint64_t destAddress,
                               std::vector<NVM::Simulation::DataBlock> data) {
    if (!timer.isAvailable(baseAddress)) return nullptr;
    timer.issue({NVM::CommandType::PIM, baseAddress});
    log() << LogLevel::EVENT << "SubArray received transverse read\n";
    totalTRs++;
    rowBufferHits = 0;
    actEnergy += 0.080096;
    return (Command*) 1;
}

Command*
SimpleSubArray::transverseWrite(uint64_t address,
                                std::vector<NVM::Simulation::DataBlock> data) {
    if (!timer.isAvailable(address)) return nullptr;
    timer.issue({NVM::CommandType::WRITE, address});
    log() << LogLevel::EVENT << "SubArray received transverse write\n";
    totalTWs++;
    totalActivates++;
    totalPrecharges++;
    rowBufferHits = 0;
    actEnergy += 0.080096;
    return (Command*) 1;
}

Command* SimpleSubArray::activate(uint64_t address) {
    if (!timer.isAvailable(address)) return nullptr;
    timer.issue({NVM::CommandType::ACTIVATE, address});
    totalActivates++;
    return (Command*) 1;
}

Command* SimpleSubArray::precharge() {
    if (!timer.isAvailable(0)) return nullptr;
    timer.issue({NVM::CommandType::PRECHARGE, 0});
    totalPrecharges++;
    return (Command*) 1;
}

Command* SimpleSubArray::refresh() { return nullptr; }

void SimpleSubArray::cycle(unsigned int cycles) { timer.cycle(); }

bool SimpleSubArray::isEmpty() const { return timer.isAvailable(0); }

StatBlock SimpleSubArray::getStats(std::string tag) const {
    StatBlock stats(tag);

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
    if (!timer.isAvailable(cmd.getAddress())) return false;
    timer.issue(cmd);

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