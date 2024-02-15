#include "MemoryTypes/Component/System/MemoryCommand.h"

using namespace NVM::ComponentType;
using NVM::Address;
using NVM::RowData;

MemoryCommand::MemoryCommand(Opcode op) : opcode(op) {}

NVM::ComponentType::MemoryCommand::MemoryCommand(Opcode op, Address a,
                                                 RowData r) :
    opcode(op),
    address(a),
    data(r) {}

NVM::ComponentType::MemoryCommand::MemoryCommand(Opcode op, Address a1,
                                                 Address a2, RowData r) :
    opcode(op),
    address(a1),
    address2(a2),
    data(r) {}

MemoryCommand::Opcode NVM::ComponentType::MemoryCommand::getOpcode() const {
    return opcode;
}

Address NVM::ComponentType::MemoryCommand::getAddress() const {
    return address;
}

Address NVM::ComponentType::MemoryCommand::getAddress2() const {
    return address2;
}

RowData NVM::ComponentType::MemoryCommand::getData() const { return data; }
