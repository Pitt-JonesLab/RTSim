#pragma once

#include "Command.h"
#include "MemoryTypes/Simple/Modeling/MemorySystem.h"
#include "MemoryTypes/Simple/Scheduling/Instruction.h"

#include <vector>

namespace NVM::Parsing {

using NVM::Command;
using NVM::Modeling::MemorySystem;
using NVM::Scheduling::Instruction;

class Parser {
    public:
    virtual std::vector<Command> parseCommands(const Instruction& transaction,
                                               MemorySystem& system) = 0;
};

} // namespace NVM::Parsing