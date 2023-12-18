#pragma once

#include "Command.h"
#include "Modeling/Types.h"

namespace NVM::Memory {

using NVM::Command;

class CommandTimer {
    public:
    virtual void cycle() = 0;
    virtual bool busy() const = 0;
    virtual void issue(NVM::Command cmd) = 0;
};

} // namespace NVM::Memory