#pragma once

#include "Modeling/MemorySystem.h"
#include "Scheduling/Instruction.h"

namespace NVM::Scheduling {

using NVM::Modeling::MemorySystem;

class Scheduler {
    public:
    virtual void enqueue(const Instruction& instruction) = 0;

    virtual const Instruction& peek(const MemorySystem& system) = 0;

    virtual Instruction issue(const MemorySystem& system) = 0;
};

} // namespace NVM::Scheduling