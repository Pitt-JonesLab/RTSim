#pragma once

#include "Memory/Timing/CommandTimer.h"

namespace NVM::Timing {

using NVM::Command;

class SimpleTimer : public NVM::Memory::CommandTimer {
    public:
    SimpleTimer();

    void cycle();
    bool busy() const;
    void issue(Command cmd);

    private:
    int remainingCycles;
};

} // namespace NVM::Timing