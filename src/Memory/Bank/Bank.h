#pragma once

#include "Memory/SubArray/SubArray.h"
#include "Simulation/MemorySystem.h"
#include "Memory/Instruction.h"

#include <memory>

namespace NVM::Memory {

class Bank {
    protected:
    std::vector<std::unique_ptr<SubArray>> subArrays;

    public:
    Command* issue(Instruction& instruction);

    virtual Command* rowClone(uint64_t srcAddress, uint64_t destAddress,
                              NVM::Simulation::DataBlock data) = 0;

    virtual Command*
    transverseRead(uint64_t baseAddress, uint64_t destAddress,
                   std::vector<NVM::Simulation::DataBlock> data) = 0;

    virtual Command*
    transverseWrite(uint64_t address,
                    std::vector<NVM::Simulation::DataBlock> data) = 0;

    virtual bool isEmpty() const = 0;

    virtual void cycle(unsigned int cycles) = 0;

    StatBlock getStats(std::string tag) const;

    void addSubArray(std::unique_ptr<SubArray> subArray);
};

} // namespace NVM::Memory