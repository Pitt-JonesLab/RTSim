#pragma once

#include <map>
#include <utility>
#include <vector>
#include <cstdint>
#include <string>

namespace NVM::Memory::Decoder {

enum class AddressSymbol {
    CHANNEL,
    RANK,
    BANK,
    ROW,
    COL
};

struct ComponentCounts {
    unsigned int channels, ranks, banks, rows, cols;
};

void setScheme(std::string order, ComponentCounts counts);

unsigned int decodeSymbol(AddressSymbol, uint64_t address);

}