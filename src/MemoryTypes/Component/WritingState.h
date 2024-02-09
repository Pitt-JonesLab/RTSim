#pragma once

#include "MemoryTypes/Component/BankState.h"

namespace NVM::ComponentType {

class WritingState : public BankState {
    public:
    WritingState(Connection<BankCommand>* cmd,
                 Connection<BankResponse>* response, unsigned int row);

    void process();
    void cycle();

    bool busy() const;

    std::unique_ptr<BankState> nextState();

    NVM::Stats::ValueStatBlock getStats();

    private:
    int delay;
    unsigned int row;
};

} // namespace NVM::ComponentType