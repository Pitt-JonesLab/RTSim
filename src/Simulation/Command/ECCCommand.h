#pragma once

#include "Simulation/Command/TraceCommand.h"

namespace NVM::Simulation {

class ECCCommand : public TraceCommand {
    public:
    ECCCommand();

    bool issue(MemorySystem* memory);
};

} // namespace NVM::Simulation