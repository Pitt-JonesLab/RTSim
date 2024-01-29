#pragma once

#include "Memory/MemorySystem.h"
#include "MemoryTypes/State/StateBank.h"

namespace NVM::State {

class StateSystem : public Memory::MemorySystem {
    public:
    bool read(const Address& address, const RowData& data);

    bool write(const Address& address, const RowData& data);

    bool rowClone(const Address& srcAddress, const Address& destAddress,
                  const RowData& data);

    bool pim(std::vector<Address> operands, const Address& destAddress,
             std::vector<RowData> data);

    bool isEmpty() const;

    void cycle(unsigned int cycles = 1);

    void printStats(std::ostream& statStream);

    private:
    StateBank bank;
};

} // namespace NVM::State