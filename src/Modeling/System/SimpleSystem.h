#pragma once

#include "Modeling/MemorySystem.h"

#include <vector>

namespace NVM::Modeling {

class Rank;

class SimpleSystem : public MemorySystem {
    public:
    SimpleSystem();

    void addRank(std::unique_ptr<Rank> rank);

    RowNumber getOpenRow(Address address) const;
    bool isOpen(Address address) const;
    void closeRow(Address address);
    void openRow(Address address);

    private:
    std::vector<std::unique_ptr<Rank>> ranks;

    Rank& decodeRank(Address address);
    const Rank& decodeRank(Address address) const;
};

} // namespace NVM::Modeling