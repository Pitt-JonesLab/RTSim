#pragma once

#include "src/TraceSimulation/DataBlock.h"
#include "src/TraceSimulation/TraceCommand.h"

#include <cstdint>

namespace NVM::Simulation {

class ReadCommand : public TraceCommand {
    public:
    ReadCommand(unsigned int cycle, uint64_t address, DataBlock data,
                unsigned int thread);

    bool issue(MemorySystem* memory);

    private:
    unsigned int cycle, threadId;
    DataBlock data;
    uint64_t address;
};

} // namespace NVM::Simulation