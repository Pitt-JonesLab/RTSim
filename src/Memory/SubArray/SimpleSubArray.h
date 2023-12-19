#pragma once

#include "Memory/SubArray/SubArray.h"
#include "Memory/Timing/CommandTimer.h"

#include <memory>

namespace NVM::Memory {

class SimpleSubArray : public SubArray {
    public:
    SimpleSubArray(unsigned int rows, std::unique_ptr<CommandTimer> timer);

    bool issue(NVM::Command cmd);

    void cycle(unsigned int cycles);

    bool isEmpty() const;

    NVM::Stats::StatBlock getStats(std::string tag) const;

    private:
    unsigned int totalReads, totalRCs, totalTRs, totalTWs;
    unsigned int totalWrites;
    unsigned int totalActivates;
    unsigned int totalPrecharges;
    unsigned int totalShifts;
    unsigned int rowBufferHits;

    double actEnergy, readEnergy, writeEnergy, shiftEnergy;

    std::unique_ptr<Command> currentCommand;
    std::unique_ptr<CommandTimer> timer;
};

} // namespace NVM::Memory