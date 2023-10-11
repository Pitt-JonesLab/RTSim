#pragma once

#include "Memory/Interconnect.h"

namespace NVM::Memory {

class SimpleInterconnect : public Interconnect {
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

    void addRank(std::unique_ptr<Rank> rank);
};

} // namespace NVM::Memory