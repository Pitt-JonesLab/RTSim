#pragma once

#include "Simulation/MemorySystem.h"
#include "Memory/Interconnect.h"

#include <memory>

namespace NVM::Memory {

class MemoryController : public NVM::Simulation::MemorySystem {
    public:
        virtual void addInterconnect(std::unique_ptr<Interconnect> interconnect) = 0;
};

} // namespace NVM::Memory