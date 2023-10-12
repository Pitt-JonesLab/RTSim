#pragma once

#include "Memory/Bank.h"

namespace NVM::Memory {

class SimpleBank : public Bank {
    public:
    Command* read(uint64_t address, NVM::Simulation::DataBlock data);

    Command* write(uint64_t address, NVM::Simulation::DataBlock data);

    void cycle(unsigned int cycles);

    bool isEmpty() const;

    void addSubArray(std::unique_ptr<SubArray> subArray);
};

} // namespace NVM::Memory