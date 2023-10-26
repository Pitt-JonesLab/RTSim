#pragma once

#include "Simulation/TraceCommand.h"

namespace NVM::Simulation {

class TransverseReadCommand : public TraceCommand {
    public:
    bool issue(MemorySystem* memory);
};

} // namespace NVM::Simulation