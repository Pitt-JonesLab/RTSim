#pragma once

#include "Memory/Rank.h"

namespace NVM::Memory {

class SimpleRank : public Rank {
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

    void addBank(std::unique_ptr<Bank> bank);
};

} // namespace NVM::Memory