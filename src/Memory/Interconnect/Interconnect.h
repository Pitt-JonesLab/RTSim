#pragma once

#include "Memory/Rank/Rank.h"
#include "Simulation/MemorySystem.h"

#include <memory>

namespace NVM::Memory {

class Interconnect : public Component {
    public:
    virtual void addRank(std::unique_ptr<Rank> rank) = 0;
};

} // namespace NVM::Memory