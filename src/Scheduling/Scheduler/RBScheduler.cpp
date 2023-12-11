#include "Scheduling/Scheduler/RBScheduler.h"

#include "Modeling/Decoder.h"

#include <algorithm>

using namespace NVM::Scheduling;
using namespace NVM::Modeling;

RBScheduler::RBScheduler() : rowBufferHits(0) {}

void RBScheduler::enqueue(const Instruction& instruction) {
    instQueue.push_back(instruction);
}

const Instruction& RBScheduler::peek(const MemorySystem& system) {
    return instQueue[0];
}

Instruction RBScheduler::issue(const MemorySystem& system) {
    auto instIter = std::find_if(
        instQueue.begin(), instQueue.end(), [&system](Instruction i) {
            if (!system.isOpen(i.getAddress())) return false;
            return system.getOpenRow(i.getAddress()) ==
                   decodeSymbol(AddressSymbol::ROW, i.getAddress());
        });

    if (instIter != instQueue.end()) {
        rowBufferHits++;
        auto inst = *instIter;
        instQueue.erase(instIter);
        return inst;
    }

    auto inst = instQueue[0];
    instQueue.erase(instQueue.begin());
    return inst;
}

bool RBScheduler::isEmpty() const { return instQueue.empty(); }

bool RBScheduler::isAvailable() const { return instQueue.size() < 20; }

int RBScheduler::getRBHits() const { return rowBufferHits; }