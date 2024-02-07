#pragma once

#include "Utils/Address.h"
#include "Utils/RowData.h"

namespace NVM::ComponentType {

class BankCommand {
    public:
    enum class Opcode { NO_OP, ACTIVATE, PRECHARGE, READ, WRITE };

    BankCommand(Opcode);
    BankCommand(Opcode, Address, RowData);

    Opcode getOpcode() const;
    Address getAddress() const;
    RowData getData() const;

    private:
    Opcode opcode;
    Address address;
    RowData data;
};

} // namespace NVM::ComponentType