#pragma once

#include "Memory/Command/Command.h"
#include "Memory/SubArray/SubArray.h"

namespace NVM::Memory {

class Instruction {
    private:
    
    uint64_t address;

    public:
    Instruction(uint64_t address);

    virtual Command* execute(SubArray&) = 0;
    uint64_t getAddress() const;
};

class ReadInstruction : public Instruction {
    private:
    NVM::Simulation::DataBlock data;
    
    public:
    ReadInstruction(uint64_t address, NVM::Simulation::DataBlock data);

    Command* execute(SubArray& subArray);
};

class WriteInstruction : public Instruction {
    private:
    NVM::Simulation::DataBlock data;
    
    public:
    WriteInstruction(uint64_t address, NVM::Simulation::DataBlock data);

    Command* execute(SubArray& subArray);
};

class ActivateInstruction : public Instruction {
    public:
    ActivateInstruction(uint64_t address);

    Command* execute(SubArray& subArray);
};

class PrechargeInstruction : public Instruction {
    public:
    PrechargeInstruction(uint64_t address);

    Command* execute(SubArray& subArray);
};

}