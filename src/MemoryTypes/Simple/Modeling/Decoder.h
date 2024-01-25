#pragma once

#include "MemoryTypes/Simple/Modeling/Types.h"

#include <string>

namespace NVM::Modeling {

enum class AddressSymbol { CHANNEL = 0, RANK, BANK, ROW, COL };

struct ComponentCounts {
    unsigned int channels, ranks, banks, rows, cols;
};

void setScheme(std::string order, ComponentCounts counts);

unsigned int decodeSymbol(AddressSymbol, Address address);

Address replaceSymbol(Address address, AddressSymbol symbol,
                      unsigned int newVal);

} // namespace NVM::Modeling