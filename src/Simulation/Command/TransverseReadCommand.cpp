#include "Simulation/Command/TransverseReadCommand.h"

#include "Simulation/MemorySystem.h"

using namespace NVM::Simulation;

TransverseReadCommand::TransverseReadCommand(uint64_t baseAddress,
                                             uint64_t destAddress) :
    baseAddress(baseAddress),
    destAddress(destAddress) {}

bool TransverseReadCommand::issue(MemorySystem* memory) {
    return memory->transverseRead(baseAddress, destAddress, {}, 0, 0);
}