#pragma once

#include "Modeling/Types.h"
#include "Parsing/Command.h"

namespace NVM::Timing {

using NVM::Modeling::Address;
using NVM::Parsing::Command;

class CommandTimer {
    public:
    virtual void cycle() = 0;
    virtual bool isAvailable(Address address) const = 0;
    virtual void issue(Command cmd) = 0;
};

} // namespace NVM::Timing