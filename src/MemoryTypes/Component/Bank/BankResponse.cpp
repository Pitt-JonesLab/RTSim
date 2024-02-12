#include "MemoryTypes/Component/Bank/BankResponse.h"

NVM::ComponentType::BankResponse::BankResponse(Opcode) {}

NVM::ComponentType::BankResponse::Opcode
NVM::ComponentType::BankResponse::getOpcode() const {
    return NVM::ComponentType::BankResponse::Opcode::NO_OP;
}
