#pragma once

#include "Memory/Bank.h"
#include "Memory/Component.h"
#include "Memory/Interconnect.h"
#include "Memory/MemoryController.h"
#include "Memory/Rank.h"
#include "Memory/SubArray.h"
#include "Simulation/DataBlock.h"
#include "Simulation/MemorySystem.h"

namespace NVM::Memory {

template<typename T> class MockComponent : public T {
    public:
    bool readFlag = false, writeFlag = false;
    bool empty = true;
    unsigned int currentCycle = 0;

    Command* read(uint64_t address, NVM::Simulation::DataBlock data) {
        readFlag = true;
        return nullptr;
    }

    Command* write(uint64_t address, NVM::Simulation::DataBlock data) {
        writeFlag = true;
        return nullptr;
    }

    bool isEmpty() const { return empty; }

    void cycle(unsigned int cycles) { currentCycle += cycles; }
};

using MockSubArray = MockComponent<SubArray>;

class MockBank : public MockComponent<Bank> {
    void addSubArray(std::unique_ptr<SubArray>) {}
};

class MockRank : public MockComponent<Rank> {
    void addBank(std::unique_ptr<Bank>) {}
};

class MockInterconnect : public MockComponent<Interconnect> {
    void addRank(std::unique_ptr<Rank>) {}
};

class MockController : public MockComponent<MemoryController> {
    void addInterconnect(std::unique_ptr<Interconnect>) {}
};

} // namespace NVM::Memory