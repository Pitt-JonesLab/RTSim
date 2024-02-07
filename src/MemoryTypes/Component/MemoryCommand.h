#pragma once

#include "Utils/Address.h"
#include "Utils/RowData.h"

namespace NVM::ComponentType {

class MemoryCommand {
    public:
    enum class Opcode { NO_OP, READ, WRITE };

    MemoryCommand(Opcode);
    MemoryCommand(Opcode, Address, RowData);

    Opcode getOpcode() const;
    Address getAddress() const;
    RowData getData() const;

    private:
    Opcode opcode;
    Address address;
    RowData data;
};

} // namespace NVM::ComponentType