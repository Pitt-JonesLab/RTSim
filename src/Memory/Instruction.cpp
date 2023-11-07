#include "Memory/Instruction.h"

using namespace NVM::Memory;

Instruction::Instruction(uint64_t address) : address(address) {}

uint64_t Instruction::getAddress() const { return address; }

ReadInstruction::ReadInstruction(uint64_t address,
                                 NVM::Simulation::DataBlock data) :
    Instruction(address),
    data(data) {}

Command* ReadInstruction::execute(SubArray& subArray) {
    return subArray.read(getAddress(), data);
}

WriteInstruction::WriteInstruction(uint64_t address,
                                   NVM::Simulation::DataBlock data) :
    Instruction(address),
    data(data) {}

Command* WriteInstruction::execute(SubArray& subArray) {
    return subArray.write(getAddress(), data);
}

ActivateInstruction::ActivateInstruction(uint64_t address) :
    Instruction(address) {}

Command* ActivateInstruction::execute(SubArray& subArray) {
    return subArray.activate(getAddress());
}

PrechargeInstruction::PrechargeInstruction(uint64_t address) :
    Instruction(address) {}

Command* PrechargeInstruction::execute(SubArray& subArray) {
    return subArray.precharge();
}