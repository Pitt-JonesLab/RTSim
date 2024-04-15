#pragma once

#include "Utils/Address.h"
#include "Utils/RowData.h"

namespace NVM::ComponentType {

class BankResponse {
    public:
    enum class Opcode { NO_OP, TR_FAILURE };

    BankResponse(Opcode);

    BankResponse(Opcode, Address, RowData);

    Opcode getOpcode() const;

    Address getAddress() const;

    RowData getData() const;

    private:
    Opcode opcode;
    Address address;
    RowData data;
};

} // namespace NVM::ComponentType