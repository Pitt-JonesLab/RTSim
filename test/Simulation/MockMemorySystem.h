#pragma once

#include "Simulation/MemorySystem.h"

namespace NVM::Simulation {
class MockMemorySystem : public MemorySystem {
    public:
    bool issue(NVMainRequest* req) { return true; }

    bool read(uint64_t address, DataBlock data, unsigned int threadId,
              unsigned int cycle) {
        lastAddress = address;
        lastData = data;
        lastThread = threadId;
        lastCycle = cycle;
        return available;
    }

    bool write(uint64_t address, DataBlock data, unsigned int threadId,
               unsigned int cycle) {
        lastAddress = address;
        lastData = data;
        lastThread = threadId;
        lastCycle = cycle;
        return available;
    }

    void cycle(unsigned int cycles) { currentCycle += cycles; }

    bool isEmpty() const { return currentCycle > lastCycle + 10; }

    void printStats(std::ostream&) {}

    unsigned int getCurrentCycle() { return currentCycle; }

    bool available = true;
    unsigned int currentCycle = 0;
    uint64_t lastAddress = 0;
    DataBlock lastData;
    unsigned int lastThread = 0;
    unsigned int lastCycle = 0;
};
} // namespace NVM::Simulation