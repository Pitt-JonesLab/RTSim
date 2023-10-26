#pragma once

#include "Memory/Bank.h"

#include <functional>
#include <vector>

namespace NVM::Memory {

class SimpleBank : public Bank {
    public:
    SimpleBank();

    Command* read(uint64_t address, NVM::Simulation::DataBlock data);

    Command* write(uint64_t address, NVM::Simulation::DataBlock data);
    Command* rowClone(uint64_t srcAddress, uint64_t destAddress,
                      NVM::Simulation::DataBlock data);
    Command* transverseRead(uint64_t baseAddress, uint64_t destAddress,
                            std::vector<NVM::Simulation::DataBlock> data);
    Command* transverseWrite(uint64_t address,
                             std::vector<NVM::Simulation::DataBlock> data);

    void cycle(unsigned int cycles);

    bool isEmpty() const;

    void addSubArray(std::unique_ptr<SubArray> subArray);

    StatBlock getStats(std::string tag) const;

    private:
    using CommandFunc = std::function<Command*()>;

    std::unique_ptr<Command> makeBankCommand(CommandFunc& func,
                                             uint64_t address);

    unsigned int totalReads;
    unsigned int totalWrites;

    std::vector<std::unique_ptr<SubArray>> subArrays;

    std::unique_ptr<Command> currentCommand;
};

} // namespace NVM::Memory