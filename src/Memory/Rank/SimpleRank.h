#pragma once

#include "Memory/Rank.h"

#include <vector>

namespace NVM::Memory {

class SimpleRank : public Rank {
    public:
    SimpleRank();

    Command* read(uint64_t address, NVM::Simulation::DataBlock data);
    Command* write(uint64_t address, NVM::Simulation::DataBlock data);

    bool isEmpty() const;

    void cycle(unsigned int cycles);

    void addBank(std::unique_ptr<Bank> bank);

    StatBlock getStats(std::string tag) const;

    private:
    unsigned int totalReads, totalWrites;

    std::vector<std::unique_ptr<Bank>> banks;
    std::unique_ptr<Command> currentCommand;
};

} // namespace NVM::Memory