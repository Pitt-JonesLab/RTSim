#include "Simulation/Command/TransverseWriteCommand.h"

#include "Simulation/MemorySystem.h"

using namespace NVM::Simulation;

bool TransverseWriteCommand::issue(MemorySystem* memory) {
    return memory->transverseWrite(0, {}, 0, 0);
}