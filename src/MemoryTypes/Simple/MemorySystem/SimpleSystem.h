#pragma once

#include "Memory/MemorySystem/MemorySystem.h"
#include "MemoryTypes/MemoryController.h"

#include <memory>
#include <vector>

namespace NVM::Memory {

using NVM::Address;
using NVM::RowData;

class SimpleSystem : public MemorySystem {
    public:
    SimpleSystem();

    void cycle(unsigned int cycles);

    unsigned int getCurrentCycle();

    bool isEmpty() const;

    void printStats(std::ostream& statStream);

    void addController(std::unique_ptr<MemoryController> controller);

    void failNext();

    bool read(const Address& address, const RowData& data);

    bool write(const Address& address, const RowData& data);

    bool rowClone(const Address& srcAddress, const Address& destAddress,
                  const RowData& data);

    bool refresh(const Address& bankAddress);

    bool pim(std::vector<Address> operands, const Address& destAddress,
             std::vector<RowData> data);

    private:
    bool available() const;

    unsigned int totalReads, totalWrites, totalRowClones, totalPIMs,
        currentCycle;

    std::vector<std::unique_ptr<MemoryController>> channels;
};

} // namespace NVM::Memory