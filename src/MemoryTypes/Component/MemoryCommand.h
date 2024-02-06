#pragma once

namespace NVM::ComponentType {

class MemoryCommand {
    public:
    enum class Opcode { NO_OP };

    MemoryCommand(Opcode);
};

} // namespace NVM::ComponentType