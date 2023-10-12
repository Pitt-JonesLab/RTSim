#pragma once

#include "Memory/Interconnect.h"

#include <vector>

namespace NVM::Memory {

class SimpleInterconnect : public Interconnect {
    public:
    Command* read(uint64_t address, NVM::Simulation::DataBlock data);

    Command* write(uint64_t address, NVM::Simulation::DataBlock data);

    void cycle(unsigned int cycles);

    bool isEmpty() const;

    void addRank(std::unique_ptr<Rank> rank);

    private:
    std::vector<std::unique_ptr<Rank>> ranks;
    std::unique_ptr<Command> currentCommand;
};

} // namespace NVM::Memory