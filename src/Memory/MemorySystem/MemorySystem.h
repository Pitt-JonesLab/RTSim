#pragma once

#include "Memory/MemoryController/MemoryController.h"
#include "Simulation/Commandable.h"

#include <memory>

namespace NVM::Memory {

class MemorySystem : public NVM::Simulation::Commandable {
    public:
    virtual void
    addController(std::unique_ptr<MemoryController> controller) = 0;

    virtual ~MemorySystem() {}
};

} // namespace NVM::Memory