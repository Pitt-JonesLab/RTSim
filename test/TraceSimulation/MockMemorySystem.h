#pragma once

#include "src/TraceSimulation/MemorySystem.h"

namespace NVM::Simulation {
class MockMemorySystem : public MemorySystem {
    public:
    bool issue(NVMainRequest* req) { return true; }

    void cycle(unsigned int cycles) {}

    bool isEmpty() const { return true; }

    void printStats(std::ostream&) {}
};
} // namespace NVM::Simulation