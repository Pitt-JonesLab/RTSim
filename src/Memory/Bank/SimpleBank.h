#pragma once

#include "Memory/Bank.h"

#include <vector>

namespace NVM::Memory {

class SimpleBank : public Bank {
    public:
    SimpleBank() = default;

    Command* read(uint64_t address, NVM::Simulation::DataBlock data);

    Command* write(uint64_t address, NVM::Simulation::DataBlock data);

    void cycle(unsigned int cycles);

    bool isEmpty() const;

    void addSubArray(std::unique_ptr<SubArray> subArray);

    StatBlock getStats(std::string tag) const;

    private:
    std::vector<std::unique_ptr<SubArray>> subArrays;

    std::unique_ptr<Command> currentCommand;
};

} // namespace NVM::Memory