#pragma once

#include "Command.h"
#include "Simulation/DataBlock.h"
#include "Stats/StatBlock.h"

#include <vector>

namespace NVM::Memory {

class SubArray {
    public:
    virtual bool issue(NVM::Command cmd) = 0;

    virtual bool isEmpty() const = 0;

    virtual void cycle(unsigned int cycles) = 0;

    virtual NVM::Stats::StatBlock getStats(std::string tag) const = 0;
};

} // namespace NVM::Memory