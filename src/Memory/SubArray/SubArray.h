#pragma once

#include "Memory/Component.h"

namespace NVM::Memory {

class SubArray {
    public:
    virtual Command* read(uint64_t address,
                          NVM::Simulation::DataBlock data) = 0;
    virtual Command* write(uint64_t address,
                           NVM::Simulation::DataBlock data) = 0;

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

    virtual Command* switchRow(unsigned int row) = 0;
};

} // namespace NVM::Memory