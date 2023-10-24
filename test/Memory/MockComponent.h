#pragma once

#include "Memory/Bank.h"
#include "Memory/Component.h"
#include "Memory/Interconnect.h"
#include "Memory/MemoryController.h"
#include "Memory/Rank.h"
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

    bool isDone() const {
        return false;
    }

    void cycle(unsigned int) {}
};

template<typename T> class MockComponent : public T {
    public:
    bool readFlag = false, writeFlag = false;
    bool empty = true;
    unsigned int currentCycle = 0;
    MockCommand command;

    Command* read(uint64_t address, NVM::Simulation::DataBlock data) {
        readFlag = true;
        return &command;
    }

    Command* write(uint64_t address, NVM::Simulation::DataBlock data) {
        writeFlag = true;
        return &command;
    }

    bool isEmpty() const { return empty; }

    void cycle(unsigned int cycles) { currentCycle += cycles; }

    StatBlock getStats(std::string tag) const { return StatBlock(); }
};

class MockSubArray : public MockComponent<SubArray> {
    Command* switchRow(unsigned int row) { return nullptr; }
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