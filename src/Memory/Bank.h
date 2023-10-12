#pragma once

#include "Memory/SubArray.h"
#include "Simulation/MemorySystem.h"

#include <memory>

namespace NVM::Memory {

class Bank : public Component {
    public:
    virtual void addSubArray(std::unique_ptr<SubArray> subArray) = 0;
};

} // namespace NVM::Memory