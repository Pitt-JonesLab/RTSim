#include "Simulation/Command/WriteCommand.h"

#include "Simulation/MemorySystem.h"

using namespace NVM::Simulation;

WriteCommand::WriteCommand(unsigned int cycle, uint64_t address, DataBlock data,
                           unsigned int thread) :
    cycle(cycle),
    address(address),
    data(data),
    threadId(thread) {}

bool WriteCommand::issue(MemorySystem* memory) {
    if (memory->getCurrentCycle() < cycle) return false;
    return memory->write(address, data, threadId, cycle);
}