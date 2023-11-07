#pragma once

#include "Memory/StatBlock.h"
#include "Memory/LowLevelComponent.h"

namespace NVM::Memory {

class SubArray : public LowLevelComponent{
    public:
    virtual bool isEmpty() const = 0;

    virtual void cycle(unsigned int cycles) = 0;

    virtual StatBlock getStats(std::string tag) const = 0;

    virtual Command* switchRow(unsigned int row) = 0;
};

} // namespace NVM::Memory