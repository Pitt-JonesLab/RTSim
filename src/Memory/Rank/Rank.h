#pragma once

#include "Memory/Bank/Bank.h"
#include "Simulation/MemorySystem.h"

#include <memory>

namespace NVM::Memory {

class Rank {
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

    virtual StatBlock getStats(std::string tag) const = 0;

    void addBank(std::unique_ptr<Bank> bank);

    protected:
    std::vector<std::unique_ptr<Bank>> banks;
};

} // namespace NVM::Memory