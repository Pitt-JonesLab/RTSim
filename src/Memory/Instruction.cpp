#include "Memory/Instruction.h"

#include "Memory/MemoryController/InstructionTranslator.h"

using namespace NVM::Memory;

Instruction::Instruction(uint64_t address, bool fails) :
    address(address),
    failsECC(fails) {}

uint64_t Instruction::getAddress() const { return address; }

bool Instruction::checkECC() const { return failsECC; }

ReadInstruction::ReadInstruction(uint64_t address,
                                 NVM::Simulation::DataBlock data, bool fails) :
    Instruction(address, fails),
    data(data) {}

Command* ReadInstruction::execute(SubArray& subArray) { return nullptr; }

std::vector<std::unique_ptr<Instruction>>
ReadInstruction::translate(InstructionTranslator& translator) {
    return translator.doRead(*this);
}

WriteInstruction::WriteInstruction(uint64_t address,
                                   NVM::Simulation::DataBlock data,
                                   bool fails) :
    Instruction(address, fails),
    data(data) {}

Command* WriteInstruction::execute(SubArray& subArray) { return nullptr; }

std::vector<std::unique_ptr<Instruction>>
WriteInstruction::translate(InstructionTranslator& translator) {
    return translator.doWrite(*this);
}

ActivateInstruction::ActivateInstruction(uint64_t address) :
    Instruction(address) {}

Command* ActivateInstruction::execute(SubArray& subArray) { return nullptr; }

std::vector<std::unique_ptr<Instruction>>
ActivateInstruction::translate(InstructionTranslator& translator) {
    return {};
}

PrechargeInstruction::PrechargeInstruction(uint64_t address) :
    Instruction(address) {}

Command* PrechargeInstruction::execute(SubArray& subArray) { return nullptr; }

std::vector<std::unique_ptr<Instruction>>
PrechargeInstruction::translate(InstructionTranslator& translator) {
    return {};
}

RowCloneInstruction::RowCloneInstruction(uint64_t srcAddress,
                                         uint64_t destAddress,
                                         NVM::Simulation::DataBlock data,
                                         bool fails) :
    Instruction(srcAddress, fails),
    destAddress(destAddress),
    data(data) {}

Command* RowCloneInstruction::execute(SubArray& subArray) { return nullptr; }

std::vector<std::unique_ptr<Instruction>>
RowCloneInstruction::translate(InstructionTranslator& translator) {
    return translator.doRowClone(*this);
}

PIMInstruction::PIMInstruction(uint64_t baseAddress, uint64_t destAddress,
                               std::vector<NVM::Simulation::DataBlock> data,
                               bool fails) :
    Instruction(baseAddress, fails),
    destAddress(destAddress),
    data(data) {}

Command* PIMInstruction::execute(SubArray& subArray) { return nullptr; }

std::vector<std::unique_ptr<Instruction>>
PIMInstruction::translate(InstructionTranslator& translator) {
    return translator.doPIM(*this);
}