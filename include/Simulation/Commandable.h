#pragma once

#include "Utils/Address.h"
#include "Utils/RowData.h"

#include <functional>
#include <vector>

namespace NVM::Simulation {

class Commandable {
    public:
    virtual bool read(const Address& address, const RowData& data) = 0;

    virtual bool write(const Address& address, const RowData& data) = 0;

    virtual bool rowClone(const Address& srcAddress, const Address& destAddress,
                          const RowData& data) = 0;

    virtual bool refresh(const Address& bankAddress) = 0;

    virtual bool pim(std::vector<Address> operands, const Address& destAddress,
                     std::vector<RowData> data) = 0;

    virtual bool isEmpty() const;

    virtual void cycle(unsigned int cycles = 1) = 0;
};

using Command = std::function<bool(Commandable&)>;

} // namespace NVM::Simulation