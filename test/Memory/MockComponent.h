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

} // namespace NVM::Memory