#include "Simulation/Command/ECCCommand.h"

#include "Simulation/MemorySystem.h"

using namespace NVM::Simulation;

ECCCommand::ECCCommand() {}

bool ECCCommand::issue(MemorySystem* memory) {
    memory->failNext();
    return true;
}