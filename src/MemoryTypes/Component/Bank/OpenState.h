#pragma once

#include "MemoryTypes/Component/Bank/Bank.h"
#include "Stats/StatBlock.h"

namespace NVM::ComponentType {

class OpenState : public State<BankInfo> {
    public:
    OpenState(BankInfo& i, unsigned int row);

    void process();
    void cycle();

    NVM::Stats::ValueStatBlock getStats();

    bool busy() const;

    private:
    unsigned int row;

    Stats::ValueStatBlock stats;
    BankResponse responseCmd;
};

} // namespace NVM::ComponentType