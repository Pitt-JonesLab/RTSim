#pragma once

namespace NVM::ComponentType {

class BankCommand {
    public:
    enum class Opcode { NO_OP };

    BankCommand(Opcode);
};

} // namespace NVM::ComponentType