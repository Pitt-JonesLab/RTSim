#pragma once

#include "Modeling/Types.h"

#include <string>

namespace NVM::Modeling {

enum class AddressSymbol { CHANNEL, RANK, BANK, ROW, COL };

struct ComponentCounts {
    unsigned int channels, ranks, banks, rows, cols;
};

void setScheme(std::string order, ComponentCounts counts);

unsigned int decodeSymbol(AddressSymbol, Address address);

} // namespace NVM::Modeling