#pragma once

#include "MemoryTypes/Component/BankState.h"
#include "Stats/StatBlock.h"

namespace NVM::ComponentType {

class OpenState : public BankState {
    public:
    OpenState(Connection<BankCommand>* cmd, Connection<BankResponse>* response,
              unsigned int row);

    void process();
    void cycle();

    std::unique_ptr<BankState> nextState();

    NVM::Stats::ValueStatBlock getStats();

    bool busy() const;

    private:
    unsigned int row;

    Stats::ValueStatBlock stats;
};

} // namespace NVM::ComponentType