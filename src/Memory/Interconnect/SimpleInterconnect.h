#pragma once

#include "Memory/Interconnect.h"

namespace NVM::Memory {

class SimpleInterconnect : public Interconnect {
    public:
    Command* read(uint64_t address, NVM::Simulation::DataBlock data);

    Command* write(uint64_t address, NVM::Simulation::DataBlock data);

    void cycle(unsigned int cycles);

    bool isEmpty() const;

    void addRank(std::unique_ptr<Rank> rank);
};

} // namespace NVM::Memory