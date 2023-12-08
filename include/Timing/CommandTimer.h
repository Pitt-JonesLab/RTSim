#pragma once

#include "Command.h"
#include "Modeling/Types.h"

namespace NVM::Timing {

using NVM::Command;
using NVM::Modeling::Address;

class CommandTimer {
    public:
    virtual void cycle() = 0;
    virtual bool isAvailable(Address address) const = 0;
    virtual void issue(Command cmd) = 0;
};

} // namespace NVM::Timing