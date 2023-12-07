#include "Scheduling/Instruction.h"

using namespace NVM::Scheduling;
using NVM::Modeling::Address;

Instruction::Instruction(InstructionType type, Address address) :
    type(type),
    address(address) {}

InstructionType Instruction::getType() const { return type; }

Address Instruction::getAddress() const { return address; }