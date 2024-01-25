#pragma once

#include "MemoryTypes/Simple/Modeling/SubArray.h"

#include <stdexcept>

namespace NVM::Modeling {

class MockSubArray : public SubArray {
    public:
    MockSubArray() : open(false), row(0) {}

    RowNumber getOpenRow() const {
        if (!open) throw std::runtime_error("");
        return row;
    }

    bool isOpen() const { return open; }

    void closeRow() {
        if (!open) throw std::runtime_error("");
        open = false;
    }

    void openRow(RowNumber r) {
        if (open) throw std::runtime_error("");
        open = true;
        row = r;
    }

    bool open;
    RowNumber row;
};

} // namespace NVM::Modeling