#pragma once

#include "Timing/CommandTimer.h"

namespace NVM::Timing {

using NVM::Command;

class SimpleTimer : public CommandTimer {
    public:
    SimpleTimer();

    void cycle();
    bool isAvailable(Address address) const;
    void issue(Command cmd);

    private:
    int remainingCycles;
};

} // namespace NVM::Timing