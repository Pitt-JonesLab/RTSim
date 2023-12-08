#pragma once

#include "Simulation/Command/TraceCommand.h"

#include <cstdint>

namespace NVM::Simulation {

class TransverseReadCommand : public TraceCommand {
    public:
    TransverseReadCommand(uint64_t baseAddress, uint64_t destAddress);

    bool issue(MemorySystem* memory);

    private:
    uint64_t baseAddress, destAddress;
};

} // namespace NVM::Simulation