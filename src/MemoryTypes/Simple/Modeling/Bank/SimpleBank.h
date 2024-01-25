#pragma once

#include "MemoryTypes/Simple/Modeling/Bank.h"

#include <vector>

namespace NVM::Modeling {

class SimpleBank : public Bank {
    public:
    SimpleBank();

    void addSubArray(std::unique_ptr<SubArray> subArray);

    RowNumber getOpenRow(Address address) const;
    bool isOpen(Address address) const;
    void closeRow(Address address);
    void openRow(Address address);

    private:
    std::vector<std::unique_ptr<SubArray>> subArrays;

    const SubArray& decodeSubArray(Address address) const;
    SubArray& decodeSubArray(Address address);
};

} // namespace NVM::Modeling