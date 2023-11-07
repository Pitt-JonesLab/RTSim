#pragma once

#include "Memory/Bank/Bank.h"
#include "Memory/Command/NullCommand.h"
#include "Memory/Component.h"
#include "Memory/Interconnect/Interconnect.h"
#include "Memory/MemoryController/MemoryController.h"
#include "Memory/Rank/Rank.h"
#include "Memory/SubArray/SubArray.h"
#include "Simulation/DataBlock.h"
#include "Simulation/MemorySystem.h"

namespace NVM::Memory {

class MockCommand : public Command {
    public:
    Command* parent;

    MockCommand() : parent(nullptr) {}

    void setParent(Command* p) { parent = p; }

    void notify() {
        if (parent) parent->notify();
        parent = nullptr;
    }

    bool isDone() const { return false; }

    void cycle(unsigned int) {}
};

template<typename T> class MockComponent : public T {
    public:
    bool readFlag = false, writeFlag = false;
    bool empty = true, available = true;
    unsigned int currentCycle = 0;
    unsigned int totalReads = 0, totalWrites = 0;
    MockCommand command;

    Command* read(uint64_t address, NVM::Simulation::DataBlock data) {
        if (!available) return nullptr;
        readFlag = true;
        totalReads++;
        return &command;
    }

    Command* write(uint64_t address, NVM::Simulation::DataBlock data) {
        if (!available) return nullptr;
        writeFlag = true;
        totalWrites++;
        return &command;
    }

    Command* rowClone(uint64_t srcAddress, uint64_t destAddress,
                      NVM::Simulation::DataBlock data) {
        return &command;
    }

    Command* transverseRead(uint64_t baseAddress, uint64_t destAddress,
                            std::vector<NVM::Simulation::DataBlock> data) {
        return &command;
    }

    Command* transverseWrite(uint64_t address,
                             std::vector<NVM::Simulation::DataBlock> data) {
        return &command;
    }

    bool isEmpty() const { return empty; }

    void cycle(unsigned int cycles) { currentCycle += cycles; }

    virtual StatBlock getStats(std::string tag) const {
        StatBlock stats(tag);

        stats.addStat(&totalReads, "reads");
        stats.addStat(&totalWrites, "writes");

        return stats;
    }
};

class MockSubArray : public MockComponent<SubArray> {
    public:
    Command* switchRow(unsigned int row) { return sendNull(); }

    Command* activate(uint64_t address) { return nullptr; }
    Command* precharge() { return nullptr; }
    Command* refresh() { return nullptr; }
};

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