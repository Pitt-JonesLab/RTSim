#include "Memory/MemoryController/InstructionTranslator.h"

#include "Memory/Decoder.h"

using namespace NVM::Memory;

InstructionTranslator::InstructionTranslator() : openRow(-1) {}

std::vector<std::unique_ptr<Instruction>>
InstructionTranslator::doRead(ReadInstruction& inst) {
    auto nextRow =
        Decoder::decodeSymbol(Decoder::AddressSymbol::ROW, inst.getAddress());

    std::vector<std::unique_ptr<Instruction>> instructions;
    if (nextRow != openRow) {
        if (openRow != -1)
            instructions.emplace_back(
                new PrechargeInstruction(inst.getAddress()));
        instructions.emplace_back(new ActivateInstruction(inst.getAddress()));
        openRow = nextRow;
    }
    instructions.emplace_back(new ReadInstruction(inst));

    return instructions;
}

std::vector<std::unique_ptr<Instruction>>
InstructionTranslator::doWrite(WriteInstruction& inst) {
    auto nextRow =
        Decoder::decodeSymbol(Decoder::AddressSymbol::ROW, inst.getAddress());

    std::vector<std::unique_ptr<Instruction>> instructions;
    if (nextRow != openRow) {
        if (openRow != -1)
            instructions.emplace_back(
                new PrechargeInstruction(inst.getAddress()));
        instructions.emplace_back(new ActivateInstruction(inst.getAddress()));
        openRow = nextRow;
    }
    instructions.emplace_back(new WriteInstruction(inst));

    return instructions;
}

std::vector<std::unique_ptr<Instruction>>
InstructionTranslator::doRowClone(RowCloneInstruction& inst) {
    std::vector<std::unique_ptr<Instruction>> instructions;

    auto nextRow =
        Decoder::decodeSymbol(Decoder::AddressSymbol::ROW, inst.getAddress());
    if (nextRow != openRow) {
        if (openRow != -1) {
            instructions.emplace_back(
                new PrechargeInstruction(inst.getAddress()));
        }
        instructions.emplace_back(new ActivateInstruction(inst.getAddress()));
    }

    instructions.emplace_back(new RowCloneInstruction(inst));

    instructions.emplace_back(new PrechargeInstruction(inst.getAddress()));
    openRow = -1;

    return instructions;
}

int InstructionTranslator::getOpenRow() const { return openRow; }