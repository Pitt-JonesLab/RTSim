#pragma once

#include "Scheduling/Scheduler.h"

#include <vector>

namespace NVM::Scheduling {

class RBScheduler : public Scheduler {
    public:
    RBScheduler();

    void enqueue(const Instruction& instruction);

    const Instruction& peek(const MemorySystem& system);

    Instruction issue(const MemorySystem& system);

    bool isEmpty() const;

    int getRBHits() const;

    private:
    std::vector<Instruction> instQueue;

    int rowBufferHits;
};

} // namespace NVM::Scheduling