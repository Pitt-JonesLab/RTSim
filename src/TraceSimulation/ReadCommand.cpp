#include "src/TraceSimulation/ReadCommand.h"

#include "src/TraceSimulation/MemorySystem.h"

using namespace NVM::Simulation;

ReadCommand::ReadCommand(unsigned int cycle, uint64_t address, DataBlock data,
                         unsigned int thread) :
    cycle(cycle),
    address(address),
    data(data),
    threadId(thread) {}

bool ReadCommand::issue(MemorySystem* memory) {
    if (memory->getCurrentCycle() < cycle) return false;
    return memory->read(address, data, threadId, cycle);
}