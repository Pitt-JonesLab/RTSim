#pragma once

#include "Memory/Command.h"
#include "Memory/StatBlock.h"
#include "Simulation/DataBlock.h"

#include <string>

namespace NVM::Memory {

class Component {
    public:
    virtual Command* read(uint64_t address,
                          NVM::Simulation::DataBlock data) = 0;
    virtual Command* write(uint64_t address,
                           NVM::Simulation::DataBlock data) = 0;

    virtual bool isEmpty() const = 0;

    virtual void cycle(unsigned int cycles) = 0;

    virtual StatBlock getStats(std::string tag) const = 0;
};

} // namespace NVM::Memory