#pragma once

#include "Modeling/MemorySystem.h"
#include "Parsing/Command.h"
#include "Scheduling/Instruction.h"

#include <vector>

namespace NVM::Parsing {

using NVM::Modeling::MemorySystem;
using NVM::Scheduling::Instruction;

class Parser {
    public:
    virtual std::vector<Command> parseCommands(const Instruction& transaction,
                                               MemorySystem& system) = 0;
};

} // namespace NVM::Parsing