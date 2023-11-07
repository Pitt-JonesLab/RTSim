#pragma once

#include "Memory/Rank/Rank.h"

#include <vector>

namespace NVM::Memory {

class SimpleRank : public Rank {
    public:
    SimpleRank();

    Command* rowClone(uint64_t srcAddress, uint64_t destAddress,
                      NVM::Simulation::DataBlock data);
    Command* transverseRead(uint64_t baseAddress, uint64_t destAddress,
                            std::vector<NVM::Simulation::DataBlock> data);
    Command* transverseWrite(uint64_t address,
                             std::vector<NVM::Simulation::DataBlock> data);

    bool isEmpty() const;

    void cycle(unsigned int cycles);

    StatBlock getStats(std::string tag) const;

    private:
    unsigned int totalReads, totalWrites;

    std::unique_ptr<Command> currentCommand;
};

} // namespace NVM::Memory