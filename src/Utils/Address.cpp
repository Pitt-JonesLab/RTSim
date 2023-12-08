#include "Utils/Address.h"

using namespace NVM;

Address::Address(uint64_t data) : data(data) {}

uint64_t Address::getBitRange(unsigned int msb, unsigned int lsb) const {
    auto range = data;
    range >>= lsb;

    uint64_t mask = 0;
    for (int i = 0; i < msb - lsb + 1; i++) {
        mask += (1 << i);
    }

    return range & mask;
}