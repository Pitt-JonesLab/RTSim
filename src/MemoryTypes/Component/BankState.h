#pragma once

#include "MemoryTypes/Component/BankCommand.h"
#include "MemoryTypes/Component/BankResponse.h"
#include "MemoryTypes/Component/Connection.h"
#include "Stats/StatBlock.h"

namespace NVM::ComponentType {

class BankState {
    public:
    BankState(Connection<BankCommand>* cmd, Connection<BankResponse>* response);

    virtual void process() = 0;
    virtual void cycle() = 0;

    virtual std::unique_ptr<BankState> nextState() = 0;

    virtual NVM::Stats::ValueStatBlock getStats() = 0;

    virtual bool busy() const = 0;

    protected:
    Connection<BankCommand>* commandConnection;
    Connection<BankResponse>* responseConnection;
    std::unique_ptr<BankState> next;
};

} // namespace NVM::ComponentType