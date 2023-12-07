#pragma once

#include "Modeling/Rank.h"

#include <vector>

namespace NVM::Modeling {

class SimpleRank : public Rank {
    public:
    SimpleRank();

    void addBank(std::unique_ptr<Bank> bank);

    RowNumber getOpenRow(Address address) const;
    bool isOpen(Address address) const;
    void closeRow(Address address);
    void openRow(Address address);

    private:
    std::vector<std::unique_ptr<Bank>> banks;

    Bank& decodeBank(Address address);
    const Bank& decodeBank(Address address) const;
};

} // namespace NVM::Modeling