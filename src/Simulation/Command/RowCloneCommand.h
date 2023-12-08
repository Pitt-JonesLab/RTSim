#pragma once

#include "Simulation/Command/TraceCommand.h"
#include "Simulation/DataBlock.h"

#include <cstdint>

namespace NVM::Simulation {

class RowCloneCommand : public TraceCommand {
    public:
    RowCloneCommand(uint64_t srcAddress, uint64_t baseAddress, DataBlock data);

    bool issue(MemorySystem* memory);

    private:
    uint64_t srcAddress, baseAddress;
    DataBlock data;
};

} // namespace NVM::Simulation