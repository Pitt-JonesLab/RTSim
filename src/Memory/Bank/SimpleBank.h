#pragma once

#include "Memory/Bank/Bank.h"

namespace NVM::Memory {

class SimpleBank : public Bank {
    public:
    void cycle(unsigned int cycles);

    bool isEmpty() const;

    NVM::Stats::StatBlock getStats(std::string tag) const;
};

} // namespace NVM::Memory