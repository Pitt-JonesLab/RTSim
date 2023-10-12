#pragma once

#include "Memory/Rank.h"

namespace NVM::Memory {

class SimpleRank : public Rank {
    public:
    Command* read(uint64_t address,
                          NVM::Simulation::DataBlock data);
    Command* write(uint64_t address,
                           NVM::Simulation::DataBlock data);

    bool isEmpty() const;

    void cycle(unsigned int cycles);

    void addBank(std::unique_ptr<Bank> bank);
};

} // namespace NVM::Memory