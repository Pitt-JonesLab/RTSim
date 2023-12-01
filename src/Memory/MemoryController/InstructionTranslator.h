#pragma once

#include "Memory/Instruction.h"

#include <memory>
#include <vector>

namespace NVM::Memory {

/**
 * Creates a list of low-level instructions from high-level instructions
 */
class InstructionTranslator {
    public:
    InstructionTranslator();

    /**
     * Converts high-level read to low-level instructions.
     *
     * Adds precharge and activate if necessary, followed by the read
     * instruction.
     *
     * @param inst Read instruction to translate
     *
     * @return Low-level instructions to execute
     */
    std::vector<std::unique_ptr<Instruction>> doRead(ReadInstruction& inst);

    /**
     * Converts high-level write to low-level instructions.
     *
     * Adds precharge and activate if necessary, followed by the write
     * instruction.
     *
     * @param inst Write instruction to translate
     *
     * @return Low-level instructions to execute
     */
    std::vector<std::unique_ptr<Instruction>> doWrite(WriteInstruction& inst);

    /**
     * Converts high-level RowClone to low-level instructions.
     *
     * Adds precharge and activate to source row if necessary, followed by
     * destination RowClone and precharge.
     *
     * @param inst RowClone instruction to translate
     *
     * @return Low-level instructions to execute
     */
    std::vector<std::unique_ptr<Instruction>>
    doRowClone(RowCloneInstruction& inst);

    /**
     * Returns currently activated row
     *
     * @return Current activated row or -1 is no row is active
     */
    int getOpenRow() const;

    private:
    int openRow;
};

} // namespace NVM::Memory