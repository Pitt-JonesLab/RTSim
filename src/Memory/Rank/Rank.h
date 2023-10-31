#pragma once

#include "Memory/Bank/Bank.h"
#include "Simulation/MemorySystem.h"

#include <memory>

namespace NVM::Memory {

class Rank : public Component {
    public:
    virtual void addBank(std::unique_ptr<Bank> bank) = 0;
};

} // namespace NVM::Memory