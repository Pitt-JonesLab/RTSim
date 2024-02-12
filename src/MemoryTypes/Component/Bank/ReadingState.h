#pragma once

#include "MemoryTypes/Component/Bank/BankState.h"

namespace NVM::ComponentType {

class ReadingState : public BankState {
    public:
    ReadingState(Connection<BankCommand>* cmd,
                 Connection<BankResponse>* response, unsigned int row);

    void process();
    void cycle();

    bool busy() const;

    NVM::Stats::ValueStatBlock getStats();

    private:
    int delay;
    unsigned int row;
};

} // namespace NVM::ComponentType