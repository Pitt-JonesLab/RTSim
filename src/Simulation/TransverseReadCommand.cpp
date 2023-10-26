#include "Simulation/TransverseReadCommand.h"

#include "Simulation/MemorySystem.h"

using namespace NVM::Simulation;

bool TransverseReadCommand::issue(MemorySystem* memory) {
    memory->transverseRead(0, 0, {}, 0, 0);
}