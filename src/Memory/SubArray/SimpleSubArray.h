#pragma once

#include "Memory/SubArray/SubArray.h"
#include "Timing/Timer/SimpleTimer.h"

#include <memory>

namespace NVM::Memory {

using NVM::Timing::SimpleTimer;

class SimpleSubArray : public SubArray {
    public:
    SimpleSubArray(unsigned int rows);

    bool issue(NVM::Command cmd);

    void cycle(unsigned int cycles);

    bool isEmpty() const;

    StatBlock getStats(std::string tag) const;

    private:
    unsigned int totalReads, totalRCs, totalTRs, totalTWs;
    unsigned int totalWrites;
    unsigned int totalActivates;
    unsigned int totalPrecharges;
    unsigned int totalShifts;
    unsigned int rowBufferHits;

    double actEnergy, readEnergy, writeEnergy, shiftEnergy;

    std::unique_ptr<Command> currentCommand;

    SimpleTimer timer;
};

} // namespace NVM::Memory