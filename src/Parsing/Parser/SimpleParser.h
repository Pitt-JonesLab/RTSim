#pragma once

#include "Parsing/Parser.h"

namespace NVM::Parsing {

class SimpleParser : public Parser {
    SimpleParser();

    std::vector<Command> parseCommands(const Instruction& transaction,
                                       MemorySystem& system);
};

} // namespace NVM::Parsing