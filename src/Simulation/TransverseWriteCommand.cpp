#include "Simulation/TransverseWriteCommand.h"

#include "Simulation/MemorySystem.h"

using namespace NVM::Simulation;

bool TransverseWriteCommand::issue(MemorySystem* memory) {
    memory->transverseWrite(0, {}, 0, 0);
}