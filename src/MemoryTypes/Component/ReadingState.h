#pragma once

#include "MemoryTypes/Component/BankState.h"

namespace NVM::ComponentType {

class ReadingState : public BankState {
    public:
    ReadingState(Connection<BankCommand>* cmd,
                 Connection<BankResponse>* response, unsigned int row);

    void process();
    void cycle();

    bool busy() const;

    NVM::Stats::ValueStatBlock getStats();

    std::unique_ptr<BankState> nextState();

    private:
    int delay;
    unsigned int row;
};

} // namespace NVM::ComponentType