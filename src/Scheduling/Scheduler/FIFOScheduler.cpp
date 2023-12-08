#include "Scheduling/Scheduler/FIFOScheduler.h"

#include <stdexcept>

using namespace NVM::Scheduling;
using NVM::Modeling::MemorySystem;

FIFOScheduler::FIFOScheduler() {}

void FIFOScheduler::enqueue(const Instruction& instruction) {
    instQueue.push_back(instruction);
}

const Instruction& FIFOScheduler::peek(const MemorySystem& system) {
    if (instQueue.empty()) throw std::runtime_error("Scheduler is empty!");
    return instQueue[0];
}

Instruction FIFOScheduler::issue(const MemorySystem& system) {
    if (instQueue.empty()) throw std::runtime_error("Scheduler is empty!");
    auto inst = instQueue[0];
    instQueue.erase(instQueue.begin());
    return inst;
}

bool FIFOScheduler::isEmpty() const { return instQueue.empty(); }