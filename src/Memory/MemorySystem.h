#pragma once

#include "Simulation/MemorySystem.h"
#include "Memory/MemoryController.h"

#include <memory>

namespace NVM::Memory {

class MemorySystem : public NVM::Simulation::MemorySystem {
    public:
        virtual void addController(std::unique_ptr<MemoryController> controller) = 0;
};

} // namespace NVM::Memory