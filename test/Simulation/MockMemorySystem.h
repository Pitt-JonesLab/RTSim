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
        readFlag = true;
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

    bool rowClone(uint64_t srcAddress, uint64_t destAddress, DataBlock data,
                  unsigned int threadId, unsigned int cycle) {
        return true;
    }

    bool transverseRead(uint64_t baseAddress, uint64_t destAddress,
                        std::vector<DataBlock> inputRows, unsigned int threadId,
                        unsigned int cycle) {
        return true;
    }

    bool transverseWrite(uint64_t baseAddress, std::vector<DataBlock> writeData,
                         unsigned int threadId, unsigned int cycle) {
        return true;
    }

    bool shift(uint64_t address, unsigned int shiftAmount, DataBlock data,
               unsigned int threadId, unsigned int cycle) {
        return true;
    }

    void cycle(unsigned int cycles) {
        currentCycle += cycles;
        available = true;
    }

    bool isEmpty() const { return currentCycle > lastCycle + 10; }

    void printStats(std::ostream&) {}

    unsigned int getCurrentCycle() { return currentCycle; }

    void failNext() {}

    bool available = true;
    unsigned int currentCycle = 0;
    uint64_t lastAddress = 0;
    DataBlock lastData;
    unsigned int lastThread = 0;
    unsigned int lastCycle = 0;
    bool readFlag = false;
};
} // namespace NVM::Simulation