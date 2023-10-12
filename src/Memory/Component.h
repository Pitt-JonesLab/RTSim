#pragma once

#include "Memory/Command.h"
#include "Simulation/DataBlock.h"

namespace NVM::Memory {

class Component {
    public:
    virtual Command* read(uint64_t address,
                          NVM::Simulation::DataBlock data) = 0;
    virtual Command* write(uint64_t address,
                           NVM::Simulation::DataBlock data) = 0;

    virtual bool isEmpty() const = 0;

    virtual void cycle(unsigned int cycles) = 0;
};

} // namespace NVM::Memory