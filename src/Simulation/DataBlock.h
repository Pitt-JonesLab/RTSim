#pragma once

#include <array>
#include <cstdint>

namespace NVM::Simulation {

struct DataBlock {
    std::array<uint8_t, 64> bytes;
};

} // namespace NVM::Simulation