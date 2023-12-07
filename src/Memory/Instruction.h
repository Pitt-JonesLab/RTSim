#pragma once

#include "Memory/Command/Command.h"
#include "Memory/SubArray/SubArray.h"

namespace NVM::Memory {

class InstructionTranslator;

class Instruction {
    private:
    uint64_t address;
    bool failsECC;

    public:
    Instruction(uint64_t address, bool fails = false);

    virtual Command* execute(SubArray&) = 0;

    virtual std::vector<std::unique_ptr<Instruction>>
    translate(InstructionTranslator& translator) = 0;

    uint64_t getAddress() const;
    bool checkECC() const;
};

class ReadInstruction : public Instruction {
    private:
    NVM::Simulation::DataBlock data;

    public:
    ReadInstruction(uint64_t address, NVM::Simulation::DataBlock data,
                    bool fails = false);

    Command* execute(SubArray& subArray);

    std::vector<std::unique_ptr<Instruction>>
    translate(InstructionTranslator& translator);
};

class WriteInstruction : public Instruction {
    private:
    NVM::Simulation::DataBlock data;

    public:
    WriteInstruction(uint64_t address, NVM::Simulation::DataBlock data,
                     bool fails = false);

    Command* execute(SubArray& subArray);

    std::vector<std::unique_ptr<Instruction>>
    translate(InstructionTranslator& translator);
};

class RowCloneInstruction : public Instruction {
    private:
    NVM::Simulation::DataBlock data;
    uint64_t destAddress;

    public:
    RowCloneInstruction(uint64_t srcAddress, uint64_t destAddress,
                        NVM::Simulation::DataBlock data, bool fails = false);

    Command* execute(SubArray& subArray);

    std::vector<std::unique_ptr<Instruction>>
    translate(InstructionTranslator& translator);
};

class ActivateInstruction : public Instruction {
    public:
    ActivateInstruction(uint64_t address);

    Command* execute(SubArray& subArray);

    std::vector<std::unique_ptr<Instruction>>
    translate(InstructionTranslator& translator);
};

class PrechargeInstruction : public Instruction {
    public:
    PrechargeInstruction(uint64_t address);

    Command* execute(SubArray& subArray);

    std::vector<std::unique_ptr<Instruction>>
    translate(InstructionTranslator& translator);
};

class PIMInstruction : public Instruction {
    public:
    PIMInstruction(uint64_t baseAddress, uint64_t destAddress,
                   std::vector<NVM::Simulation::DataBlock> data,
                   bool fails = false);

    Command* execute(SubArray& subArray);

    std::vector<std::unique_ptr<Instruction>>
    translate(InstructionTranslator& translator);

    private:
    uint64_t destAddress;
    std::vector<NVM::Simulation::DataBlock> data;
};

} // namespace NVM::Memory