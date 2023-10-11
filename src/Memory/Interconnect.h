#pragma once

#include "Memory/Rank.h"
#include "Simulation/MemorySystem.h"

#include <memory>

namespace NVM::Memory {

class Interconnect : public NVM::Simulation::MemorySystem {
    public:
    virtual void addRank(std::unique_ptr<Rank> rank) = 0;
};

} // namespace NVM::Memory