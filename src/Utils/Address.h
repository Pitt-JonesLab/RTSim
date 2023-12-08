#pragma once

#include <cstdint>

namespace NVM {

class Address {
    public:
    Address(uint64_t data);

    uint64_t getBitRange(unsigned int msb, unsigned int lsb) const;

    private:
    uint64_t data;
};

} // namespace NVM