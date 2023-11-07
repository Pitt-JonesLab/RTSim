#pragma once

#include "Memory/Rank/Rank.h"

#include <vector>

namespace NVM::Memory {

class SimpleRank : public Rank {
    public:
    bool isEmpty() const;

    void cycle(unsigned int cycles);

    StatBlock getStats(std::string tag) const;
};

} // namespace NVM::Memory