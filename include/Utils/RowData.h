#pragma once

#include <array>
#include <cstdint>
#include <cstdlib>

namespace NVM {

/*
 *   Binary data for a row
 *   Used to calculate energy cost for certain operations
 *   Currently 512 bits, but this should be configurable (TODO)
 */
class RowData {
    public:
    RowData();

    void setByte(size_t index, uint8_t byte);
    uint8_t getByte(size_t index) const;

    private:
    std::array<uint8_t, 64> data;
};

} // namespace NVM