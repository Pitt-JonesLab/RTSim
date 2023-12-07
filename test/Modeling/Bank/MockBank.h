#pragma once

#include "Modeling/Bank.h"
#include "Modeling/Decoder.h"

namespace NVM::Modeling {

class MockBank : public Bank {
    public:
    MockBank() : open(false), row(0) {}

    bool open;
    RowNumber row;

    void addSubArray(std::unique_ptr<SubArray> subArray) {}

    RowNumber getOpenRow(Address address) const { return row; }

    bool isOpen(Address address) const { return open; }

    void closeRow(Address address) { open = false; }

    void openRow(Address address) {
        open = true;
        row = decodeSymbol(AddressSymbol::ROW, address);
    }
};

} // namespace NVM::Modeling