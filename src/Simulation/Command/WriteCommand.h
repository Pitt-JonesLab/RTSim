#pragma once

#include "Simulation/Command/TraceCommand.h"
#include "Simulation/DataBlock.h"

#include <cstdint>

namespace NVM::Simulation {

class WriteCommand : public TraceCommand {
    public:
    WriteCommand(unsigned int cycle, uint64_t address, DataBlock data,
                 unsigned int thread);

    bool issue(MemorySystem* memory);

    private:
    uint64_t address;
    DataBlock data;
    unsigned int cycle, threadId;
};

} // namespace NVM::Simulation