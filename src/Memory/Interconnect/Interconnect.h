#pragma once

#include "Memory/Rank/Rank.h"
#include "Simulation/MemorySystem.h"


#include <memory>

namespace NVM::Memory {

class Interconnect {
    public:
    Command* issueInst(Instruction& instruction);

    bool issue(NVM::Command cmd);

    virtual bool isEmpty() const = 0;

    virtual void cycle(unsigned int cycles) = 0;

    StatBlock getStats(std::string tag) const;

    void addRank(std::unique_ptr<Rank> rank);

    protected:
    std::vector<std::unique_ptr<Rank>> ranks;
};

} // namespace NVM::Memory