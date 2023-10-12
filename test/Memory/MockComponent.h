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
    bool canIssue = false, canRead = false, canWrite = false;
    bool empty = true;
    unsigned int currentCycle = 0;

    bool readFlag = false, writeFlag = false;

    bool issue(NVMainRequest* req) { return canIssue; }

    bool read(uint64_t address, NVM::Simulation::DataBlock data,
              unsigned int threadId, unsigned int cycle) {
        readFlag = true;
        return canRead;
    }

    bool write(uint64_t address, NVM::Simulation::DataBlock data,
               unsigned int threadId, unsigned int cycle) {
        writeFlag = true;
        return canWrite;
    }

    void cycle(unsigned int cycles) { currentCycle += cycles; }

    unsigned int getCurrentCycle() { return currentCycle; }

    bool isEmpty() const { return empty; }

    void printStats(std::ostream& statStream) {}
};

template<typename T> class MockComponent2 : public T {
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

using MockSubArray = MockComponent2<SubArray>;

class MockBank : public MockComponent2<Bank> {
    void addSubArray(std::unique_ptr<SubArray>) {}
};

class MockRank : public MockComponent2<Rank> {
    void addBank(std::unique_ptr<Bank>) {}
};

class MockInterconnect : public MockComponent2<Interconnect> {
    void addRank(std::unique_ptr<Rank>) {}
};

class MockController : public MockComponent<MemoryController> {
    void addInterconnect(std::unique_ptr<Interconnect>) {}
};

} // namespace NVM::Memory