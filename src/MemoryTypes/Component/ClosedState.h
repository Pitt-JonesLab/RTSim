#pragma once

#include "MemoryTypes/Component/BankState.h"
#include "Stats/StatBlock.h"

namespace NVM::ComponentType {

class ClosedState : public BankState {
    public:
    ClosedState(Connection<BankCommand>* cmd,
                Connection<BankResponse>* response);

    NVM::Stats::ValueStatBlock getStats();

    void process();
    void cycle();

    bool busy() const;

    private:
    Stats::ValueStatBlock stats;
};

} // namespace NVM::ComponentType