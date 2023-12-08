#include "Simulation/Command/RowCloneCommand.h"

#include "Simulation/MemorySystem.h"

#include <cstdint>

using namespace NVM::Simulation;

RowCloneCommand::RowCloneCommand(uint64_t srcAddress, uint64_t baseAddress,
                                 DataBlock data) :
    srcAddress(srcAddress),
    baseAddress(baseAddress),
    data(data) {}

bool RowCloneCommand::issue(MemorySystem* memory) {
    return memory->rowClone(srcAddress, baseAddress, data, 0, 0);
}