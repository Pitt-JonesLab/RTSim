#pragma once

#include "Utils/RowData.h"

#include <array>
#include <cstdint>

namespace NVM::Simulation {

struct DataBlock {
    DataBlock() : bytes({0}) {}

    DataBlock(std::array<uint8_t, 64> d) : bytes(d) {}

    DataBlock(const RowData& rowData) {
        for (int i = 0; i < 64; i++) bytes[i] = rowData.getByte(i);
    }

    std::array<uint8_t, 64> bytes;
};

} // namespace NVM::Simulation