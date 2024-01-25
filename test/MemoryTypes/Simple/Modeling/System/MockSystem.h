#pragma once

#include "MemoryTypes/Simple/Modeling/Decoder.h"
#include "MemoryTypes/Simple/Modeling/MemorySystem.h"

namespace NVM::Modeling {

class MockSystem : public MemorySystem {
    public:
    RowNumber row;
    bool open;

    MockSystem() : row(0), open(false) {}

    RowNumber getOpenRow(Address address) const { return row; }

    bool isOpen(Address address) const { return open; }

    void closeRow(Address address) { open = false; }

    void openRow(Address address) {
        row = decodeSymbol(AddressSymbol::ROW, address);
        open = true;
    }
};

} // namespace NVM::Modeling