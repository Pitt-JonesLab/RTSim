#pragma once

#include "Simulation/TraceCommand.h"

namespace NVM::Simulation {

class RowCloneCommand : public TraceCommand {
    public:
    bool issue(MemorySystem* memory);
};

} // namespace NVM::Simulation