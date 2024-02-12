#pragma once

#include "MemoryTypes/Component/Bank/BankCommand.h"
#include "MemoryTypes/Component/Bank/BankResponse.h"
#include "MemoryTypes/Component/Bank/BankState.h"
#include "MemoryTypes/Component/Component.h"
#include "MemoryTypes/Component/Connection.h"
#include "MemoryTypes/Component/State/StateMachine.h"
#include "Stats/StatBlock.h"

namespace NVM::ComponentType {

class Bank : public StateMachine {
    public:
    Bank();

    void setResponseConnection(Connection<BankResponse>* connection);

    void setCommandConnection(Connection<BankCommand>* connection);

    bool busy() const;

    private:
    Connection<BankResponse>* responseConnection;
    Connection<BankCommand>* commandConnection;
};

} // namespace NVM::ComponentType