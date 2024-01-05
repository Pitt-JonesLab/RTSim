#pragma once

#include "Command.h"

namespace NVM::Memory {

using NVM::Command;

class CommandTimer {
    public:
    virtual void cycle() = 0;
    virtual bool busy() const = 0;
    virtual void issue(NVM::Command cmd, unsigned int copies) = 0;
    virtual void addFaultDelay(NVM::Command cmd) = 0;
    virtual void addDelay(unsigned int delayCycles) = 0;
};

} // namespace NVM::Memory