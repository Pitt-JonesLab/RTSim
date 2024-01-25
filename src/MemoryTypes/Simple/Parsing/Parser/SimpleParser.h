#pragma once

#include "MemoryTypes/Simple/Parsing/Parser.h"

namespace NVM::Parsing {

using NVM::Command;

class SimpleParser : public Parser {
    public:
    SimpleParser();

    std::vector<Command> parseCommands(const Instruction& transaction,
                                       MemorySystem& system);
};

} // namespace NVM::Parsing