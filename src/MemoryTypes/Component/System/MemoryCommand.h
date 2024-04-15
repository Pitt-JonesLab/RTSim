#pragma once

#include "Utils/Address.h"
#include "Utils/RowData.h"

namespace NVM::ComponentType {

class MemoryCommand {
    public:
    enum class Opcode { NO_OP, READ, WRITE, ROWCLONE, TRANSVERSE_READ };

    MemoryCommand(Opcode);
    MemoryCommand(Opcode, Address, RowData);
    MemoryCommand(Opcode, Address, Address, RowData);

    Opcode getOpcode() const;
    Address getAddress() const;
    Address getAddress2() const;
    RowData getData() const;

    private:
    Opcode opcode;
    Address address, address2;
    RowData data;
};

} // namespace NVM::ComponentType