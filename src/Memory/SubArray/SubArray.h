#pragma once

#include "Command.h"
#include "Memory/StatBlock.h"
#include "Simulation/DataBlock.h"

#include <vector>

namespace NVM::Memory {

class SubArray {
    public:
    using DataBlock = NVM::Simulation::DataBlock;
    using Address = uint64_t;

    virtual bool issue(NVM::Command cmd) = 0;

    virtual bool isEmpty() const = 0;

    virtual void cycle(unsigned int cycles) = 0;

    virtual StatBlock getStats(std::string tag) const = 0;
};

} // namespace NVM::Memory