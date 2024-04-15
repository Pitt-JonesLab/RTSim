#pragma once

#include "MemoryTypes/Component/Bank/Bank.h"

namespace NVM::ComponentType {

class ReadingState : public State<BankInfo> {
    public:
    ReadingState(BankInfo& i, unsigned int row);

    void process();
    void cycle();

    bool busy() const;

    NVM::Stats::ValueStatBlock getStats();

    private:
    int delay;
    unsigned int row;
};

} // namespace NVM::ComponentType