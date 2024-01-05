#pragma once

#include "Scheduling/Scheduler.h"

#include <vector>

namespace NVM::Scheduling {

class FIFOScheduler : public Scheduler {
    public:
    FIFOScheduler();

    void enqueue(const Instruction& instruction);

    const Instruction& peek(const MemorySystem& system);

    Instruction issue(const MemorySystem& system);

    bool isEmpty() const;

    bool isAvailable() const;

    private:
    std::vector<Instruction> instQueue;
};

} // namespace NVM::Scheduling