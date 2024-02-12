#include "MemoryTypes/Component/Bank/BankCommand.h"

using NVM::Address;
using NVM::RowData;

using namespace NVM::ComponentType;

NVM::ComponentType::BankCommand::BankCommand(Opcode op) : opcode(op) {}

NVM::ComponentType::BankCommand::BankCommand(Opcode op, Address a, RowData d) :
    opcode(op),
    address(a),
    data(d) {}

BankCommand::Opcode NVM::ComponentType::BankCommand::getOpcode() const {
    return opcode;
}

Address NVM::ComponentType::BankCommand::getAddress() const { return address; }

RowData NVM::ComponentType::BankCommand::getData() const { return data; }
