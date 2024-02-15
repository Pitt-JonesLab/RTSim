#pragma once

#include "MemoryTypes/Component/Bank/Bank.h"

namespace NVM::ComponentType {

class WritingState : public State<BankInfo> {
    public:
    WritingState(BankInfo& i, unsigned int row);

    void process();
    void cycle();

    bool busy() const;

    NVM::Stats::ValueStatBlock getStats();

    private:
    int delay;
    unsigned int row;
};

} // namespace NVM::ComponentType