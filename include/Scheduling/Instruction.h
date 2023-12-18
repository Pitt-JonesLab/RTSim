#pragma once

#include "Modeling/Types.h"

namespace NVM::Scheduling {

enum class InstructionType { READ, WRITE, ROWCLONE, PIM };

using NVM::Modeling::Address;

class Instruction {
    public:
    Instruction(InstructionType type, Address address);

    InstructionType getType() const;
    Address getAddress() const;

    private:
    InstructionType type;
    Address address;
};

} // namespace NVM::Scheduling