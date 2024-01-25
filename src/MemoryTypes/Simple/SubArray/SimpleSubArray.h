#pragma once

#include "Memory/Timing/CommandTimer.h"
#include "MemoryTypes/Simple/FaultModel.h"
#include "MemoryTypes/SubArray.h"

#include <memory>

namespace NVM::Memory {

class SimpleSubArray : public SubArray {
    public:
    SimpleSubArray(unsigned int rows, std::unique_ptr<CommandTimer> timer,
                   FaultModel faultModel, unsigned int numTries);

    bool issue(NVM::Command cmd);

    void cycle(unsigned int cycles);

    bool isEmpty() const;

    NVM::Stats::StatBlock getStats(std::string tag) const;

    private:
    void checkFaults();

    unsigned int totalReads, totalRCs, totalTRs, totalTWs;
    unsigned int totalWrites;
    unsigned int totalActivates;
    unsigned int totalPrecharges;
    unsigned int totalShifts;
    unsigned int rowBufferHits;
    unsigned int numTries;
    unsigned int numFaults;

    double actEnergy, readEnergy, writeEnergy, shiftEnergy;

    FaultModel faultModel;

    std::unique_ptr<Command> currentCommand;
    std::unique_ptr<CommandTimer> timer;
};

} // namespace NVM::Memory