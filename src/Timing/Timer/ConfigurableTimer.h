#pragma once

#include "Memory/Timing/CommandTimer.h"

#include "Simulation/Config.h"

#include <array>

namespace NVM::Timing {

using NVM::Simulation::Config;
using NVM::Command;

class ConfigurableTimer : public NVM::Memory::CommandTimer {
    public:
    ConfigurableTimer(const Config& conf);

    void cycle();
    bool busy() const;
    void issue(NVM::Command cmd);

    private:
    std::array<unsigned int, NVM::numCommandTypes> timings;
    unsigned int remainingCycles;
};

}