#pragma once

#include "MemoryTypes/Simple/Modeling/Decoder.h"
#include "MemoryTypes/Simple/Modeling/Rank.h"

namespace NVM::Modeling {

class MockRank : public Rank {
    public:
    bool open;
    RowNumber row;

    MockRank() : open(false), row(0) {}

    void addBank(std::unique_ptr<Bank> bank) {}

    RowNumber getOpenRow(Address address) const { return row; }

    bool isOpen(Address address) const { return open; }

    void closeRow(Address address) { open = false; }

    void openRow(Address address) {
        auto newRow = decodeSymbol(AddressSymbol::ROW, address);
        open = true;
        row = newRow;
    }
};

} // namespace NVM::Modeling