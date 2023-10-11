#pragma once

#include "Memory/Bank.h"

namespace NVM::Memory {

class SimpleBank : public Bank {
    public:
    bool issue(NVMainRequest* req);

    bool read(uint64_t address, NVM::Simulation::DataBlock data,
              unsigned int threadId, unsigned int cycle);
    bool write(uint64_t address, NVM::Simulation::DataBlock data,
               unsigned int threadId, unsigned int cycle);

    void cycle(unsigned int cycles);

    unsigned int getCurrentCycle();

    bool isEmpty() const;

    void printStats(std::ostream& statStream);

    void addSubArray(std::unique_ptr<SubArray> subArray);
};

} // namespace NVM::Memory