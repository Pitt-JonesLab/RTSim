#pragma once

#include "Modeling/Types.h"

#include <memory>

namespace NVM::Modeling {

class Bank;

class Rank {
    public:
    virtual void addBank(std::unique_ptr<Bank> bank) = 0;

    virtual RowNumber getOpenRow(Address address) const = 0;
    virtual bool isOpen(Address address) const = 0;
    virtual void closeRow(Address address) = 0;
    virtual void openRow(Address address) = 0;
};

} // namespace NVM::Modeling