#pragma once

#include "MemoryTypes/Simple/Modeling/Types.h"

namespace NVM::Modeling {

class SubArray {
    public:
    virtual RowNumber getOpenRow() const = 0;
    virtual bool isOpen() const = 0;
    virtual void closeRow() = 0;
    virtual void openRow(RowNumber row) = 0;
};

} // namespace NVM::Modeling