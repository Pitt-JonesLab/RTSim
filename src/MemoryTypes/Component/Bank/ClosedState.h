#pragma once

#include "MemoryTypes/Component/Bank/Bank.h"
#include "Stats/StatBlock.h"

namespace NVM::ComponentType {

class ClosedState : public State<BankInfo> {
    public:
    ClosedState(BankInfo& i);

    NVM::Stats::ValueStatBlock getStats();

    void process();
    void cycle();

    bool busy() const;

    private:
    Stats::ValueStatBlock stats;
};

} // namespace NVM::ComponentType