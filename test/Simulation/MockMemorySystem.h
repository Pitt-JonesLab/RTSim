#pragma once

#include "Memory/MemorySystem.h"

namespace NVM::Simulation {

using NVM::Address;
using NVM::RowData;

class MockMemorySystem : public NVM::Memory::MemorySystem {
    public:
    bool read(const Address& address, const RowData& data) {
        lastAddress = address;
        lastData = data;
        readFlag = true;
        return available;
    }

    bool write(const Address& address, const RowData& data) {
        lastAddress = address;
        lastData = data;
        return available;
    }

    bool rowClone(const Address& srcAddress, const Address& destAddress,
                  const RowData& data) {
        lastAddress = srcAddress;
        lastData = data;
        return available;
    }

    bool refresh(const Address& bankAddress) {
        lastAddress = bankAddress;
        return available;
    }

    bool pim(std::vector<Address> operands, const Address& destAddress,
             std::vector<RowData> data) {
        return available;
    }

    bool transverseWrite(const Address& address, const RowData& data) {
        return available;
    }

    bool isEmpty() const { return currentCycle > lastCycle + 10; }

    void cycle(unsigned int cycles = 1) {
        currentCycle += cycles;
        if (working) available = true;
    }

    void printStats(std::ostream& statStream) {}

    bool available = true, working = true;
    unsigned int currentCycle = 0;
    Address lastAddress = 0;
    RowData lastData;
    unsigned int lastThread = 0;
    unsigned int lastCycle = 0;
    bool readFlag = false;
};
} // namespace NVM::Simulation