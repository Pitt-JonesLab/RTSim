#pragma once

#include "Memory/Interconnect/Interconnect.h"
#include "Simulation/MemorySystem.h"

#include <memory>

namespace NVM::Memory {

class MemoryController : public Component {
    public:
    virtual void
    addInterconnect(std::unique_ptr<Interconnect> interconnect) = 0;
};

} // namespace NVM::Memory