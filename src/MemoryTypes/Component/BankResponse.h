#pragma once

namespace NVM::ComponentType {

class BankResponse {
    public:
    enum class Opcode { NO_OP };

    BankResponse(Opcode);
};

} // namespace NVM::ComponentType