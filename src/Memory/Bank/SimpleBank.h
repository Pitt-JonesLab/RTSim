#pragma once

#include "Memory/Bank/Bank.h"

#include <functional>
#include <vector>

namespace NVM::Memory {

class SimpleBank : public Bank {
    public:
    void cycle(unsigned int cycles);

    bool isEmpty() const;

    StatBlock getStats(std::string tag) const;
};

} // namespace NVM::Memory