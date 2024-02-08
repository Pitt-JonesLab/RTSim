#pragma once

#include "MemoryTypes/Component/BankCommand.h"
#include "MemoryTypes/Component/BankModel.h"
#include "MemoryTypes/Component/MemoryCommand.h"

#include <vector>

namespace NVM::ComponentType {

class Parser {
    public:
    Parser() = default;

    std::vector<BankCommand> parse(MemoryCommand cmd, BankModel& bank);
};

} // namespace NVM::ComponentType