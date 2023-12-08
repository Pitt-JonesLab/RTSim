#pragma once

#include "Command.h"
#include "Memory/Instruction.h"
#include "Memory/SubArray/SubArray.h"
#include "Simulation/MemorySystem.h"

#include <memory>

namespace NVM::Memory {

class Bank {
    protected:
    std::vector<std::unique_ptr<SubArray>> subArrays;

    public:
    Command* issueInst(Instruction& instruction);

    bool issue(NVM::Command cmd);

    virtual bool isEmpty() const = 0;

    virtual void cycle(unsigned int cycles) = 0;

    StatBlock getStats(std::string tag) const;

    void addSubArray(std::unique_ptr<SubArray> subArray);
};

} // namespace NVM::Memory