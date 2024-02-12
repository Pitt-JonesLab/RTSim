#pragma once

#include "MemoryTypes/Component/Bank/BankState.h"
#include "Stats/StatBlock.h"

namespace NVM::ComponentType {

class OpenState : public BankState {
    public:
    OpenState(Connection<BankCommand>* cmd, Connection<BankResponse>* response,
              unsigned int row);

    void process();
    void cycle();

    NVM::Stats::ValueStatBlock getStats();

    bool busy() const;

    private:
    unsigned int row;

    Stats::ValueStatBlock stats;
};

} // namespace NVM::ComponentType