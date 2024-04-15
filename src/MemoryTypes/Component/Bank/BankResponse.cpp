#include "MemoryTypes/Component/Bank/BankResponse.h"

using namespace NVM;

NVM::ComponentType::BankResponse::BankResponse(Opcode op) : opcode(op) {}

NVM::ComponentType::BankResponse::BankResponse(Opcode op, Address a,
                                               RowData rd) :
    opcode(op),
    address(a),
    data(rd) {}

NVM::ComponentType::BankResponse::Opcode
NVM::ComponentType::BankResponse::getOpcode() const {
    return opcode;
}

Address NVM::ComponentType::BankResponse::getAddress() const { return address; }

RowData NVM::ComponentType::BankResponse::getData() const { return data; }
