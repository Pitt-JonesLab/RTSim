#pragma once

#include "Modeling/Types.h"

#include <memory>

namespace NVM::Modeling {

class SubArray;

class Bank {
    public:
    virtual void addSubArray(std::unique_ptr<SubArray> subArray) = 0;

    virtual RowNumber getOpenRow(Address address) const = 0;
    virtual bool isOpen(Address address) const = 0;
    virtual void closeRow(Address address) = 0;
    virtual void openRow(Address address, RowNumber row) = 0;
};

} // namespace NVM::Modeling