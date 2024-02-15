#pragma once

#include "MemoryTypes/Component/Bank/BankCommand.h"
#include "MemoryTypes/Component/Bank/BankResponse.h"
#include "MemoryTypes/Component/Component.h"
#include "MemoryTypes/Component/Connection.h"
#include "MemoryTypes/Component/State/StateMachine.h"
#include "Stats/StatBlock.h"

namespace NVM::ComponentType {

struct EnergyCosts {
    double readEnergy, writeEnergy, activateEnergy, copyEnergy,
        transverseReadEnergy;
};

struct BankInfo {
    Connection<BankResponse>* responseConnection;
    Connection<BankCommand>* commandConnection;
    EnergyCosts energyCosts;
};

class Bank : public StateMachine<BankInfo> {
    public:
    Bank();

    void setResponseConnection(Connection<BankResponse>* connection);

    void setCommandConnection(Connection<BankCommand>* connection);

    bool busy() const;

    private:
    // Connection<BankResponse>* responseConnection;
    // Connection<BankCommand>* commandConnection;

    // EnergyCosts energyCosts;
};

} // namespace NVM::ComponentType