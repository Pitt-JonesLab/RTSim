#pragma once

#include "Modeling/Types.h"

#include <memory>

namespace NVM::Modeling {

class Rank;

class MemorySystem {
    public:
    virtual void addRank(std::unique_ptr<Rank> rank) = 0;

    virtual RowNumber getOpenRow(Address address) const = 0;
    virtual bool isOpen(Address address) const = 0;
    virtual void closeRow(Address address) = 0;
    virtual void openRow(Address address, RowNumber row) = 0;
};

} // namespace NVM::Modeling