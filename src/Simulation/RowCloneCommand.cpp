#include "Simulation/RowCloneCommand.h"

#include "Simulation/MemorySystem.h"

using namespace NVM::Simulation;

bool RowCloneCommand::issue(MemorySystem* memory) {
    return memory->rowClone(0, 0, DataBlock(), 0, 0);
}