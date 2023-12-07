#pragma once

#include "Timing/CommandTimer.h"

namespace NVM::Timing {

class SimpleTimer : public CommandTimer {
    public:
    SimpleTimer();

    void cycle();
    bool isAvailable(Address address) const;
    void issue(Command cmd);
};

} // namespace NVM::Timing