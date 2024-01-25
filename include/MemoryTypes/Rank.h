#pragma once

#include "Command.h"
#include "MemoryTypes/Bank.h"

#include <memory>

namespace NVM::Memory {

class Rank {
    public:
    bool issue(NVM::Command cmd);

    virtual bool isEmpty() const = 0;

    virtual void cycle(unsigned int cycles) = 0;

    virtual NVM::Stats::StatBlock getStats(std::string tag) const = 0;

    void addBank(std::unique_ptr<Bank> bank);

    protected:
    std::vector<std::unique_ptr<Bank>> banks;
};

} // namespace NVM::Memory