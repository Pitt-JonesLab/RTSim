#pragma once

#include "MemoryTypes/Component/BankCommand.h"
#include "MemoryTypes/Component/BankResponse.h"
#include "MemoryTypes/Component/Connection.h"

namespace NVM::ComponentType {

class BankState {
    public:
    BankState(Connection<BankCommand>* cmd, Connection<BankResponse>* response);

    virtual void process() = 0;
    virtual void cycle() = 0;

    virtual std::unique_ptr<BankState> nextState() = 0;

    protected:
    Connection<BankCommand>* commandConnection;
    Connection<BankResponse>* responseConnection;
};

} // namespace NVM::ComponentType