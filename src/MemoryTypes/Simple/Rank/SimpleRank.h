#pragma once

#include "MemoryTypes/Rank.h"

namespace NVM::Memory {

class SimpleRank : public Rank {
    public:
    bool isEmpty() const;

    void cycle(unsigned int cycles);

    NVM::Stats::StatBlock getStats(std::string tag) const;
};

} // namespace NVM::Memory