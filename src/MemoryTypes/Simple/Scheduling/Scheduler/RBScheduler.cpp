#include "MemoryTypes/Simple/Scheduling/Scheduler/RBScheduler.h"

#include "MemoryTypes/Simple/Modeling/Decoder.h"

#include <algorithm>
#include <stdexcept>

using namespace NVM::Scheduling;
using namespace NVM::Modeling;

/**
 * Constructor for RBScheduler.
 * initialize rowBufferHits to 0.
 */
RBScheduler::RBScheduler() : rowBufferHits(0) {}

/**
 * Enqueue an instruction to the instruction queue.
 * @param instruction the instruction to enqueue.
 */
void RBScheduler::enqueue(const Instruction& instruction) {
    instQueue.push_back(instruction);
}

/**
 * Peek the instruction queue.
 * @param system the memory system.
 * @return the instruction at the front of the queue.
 */
const Instruction& RBScheduler::peek(const MemorySystem& system) {
    if (instQueue.empty()) throw std::runtime_error("Scheduler is empty!");
    return instQueue[0];
}

/**
 * Issue an instruction from the instruction queue.
 * Checks for row buffer hits first.
 * @param system the memory system.
 * @return the instruction to issue.

*/
Instruction RBScheduler::issue(const MemorySystem& system) {
    if (instQueue.empty()) throw std::runtime_error("Scheduler is empty!");
    // Check for first row buffer hit
    // a row buffer hit is when the row buffer is open and the instruction is
    // requesting the same row
    auto instIter = std::find_if(
        instQueue.begin(), instQueue.end(), [&system](Instruction i) {
            if (!system.isOpen(i.getAddress())) return false;
            return system.getOpenRow(i.getAddress()) ==
                   decodeSymbol(AddressSymbol::ROW, i.getAddress());
        });
    // If there is a row buffer hit, issue that instruction
    if (instIter != instQueue.end()) {
        rowBufferHits++;
        auto inst = *instIter;
        instQueue.erase(instIter);
        return inst;
    }
    // Otherwise, issue the first instruction in the queue
    auto inst = instQueue[0];
    instQueue.erase(instQueue.begin());
    return inst;
}

/**
 * Check if the instruction queue is empty.
 * @return true if the instruction queue is empty, false otherwise.
 */
bool RBScheduler::isEmpty() const { return instQueue.empty(); }

/**
 * Check if the instruction queue is available.
 * @return true if the instruction queue is available, false otherwise.
 */
bool RBScheduler::isAvailable() const { return instQueue.size() < 20; }

/**
 * Get the number of row buffer hits.
 * @return the number of row buffer hits.
 */
int RBScheduler::getRBHits() const { return rowBufferHits; }