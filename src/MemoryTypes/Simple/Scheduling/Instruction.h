#pragma once

#include "MemoryTypes/Simple/Modeling/Types.h"

namespace NVM::Scheduling {

enum class InstructionType { READ, WRITE, ROWCLONE, PIM, TRANSVERSE_WRITE };

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