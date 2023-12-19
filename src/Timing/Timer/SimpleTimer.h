#pragma once

#include "Memory/Timing/CommandTimer.h"

namespace NVM::Timing {

using NVM::Command;

class SimpleTimer : public NVM::Memory::CommandTimer {
    public:
    SimpleTimer();

    void cycle();
    bool busy() const;
    void issue(Command cmd, unsigned int numTries);
    void addFaultDelay(Command cmd);
    void addDelay(unsigned int delayCycles);

    private:
    int remainingCycles;
};

} // namespace NVM::Timing