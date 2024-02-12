#pragma once

#include "MemoryTypes/Component/BankCommand.h"
#include "MemoryTypes/Component/BankResponse.h"
#include "MemoryTypes/Component/BankState.h"
#include "MemoryTypes/Component/Component.h"
#include "MemoryTypes/Component/Connection.h"
#include "MemoryTypes/Component/StateMachine.h"
#include "Stats/StatBlock.h"

namespace NVM::ComponentType {

class Bank : public Component, StateMachine {
    public:
    Bank();

    /*
     *  Processes incoming commands from upstream Components
     */
    void process();

    /*
     *  Sends outgoing commands to downstream Components
     */
    void cycle();

    /*
     *  Returns all stat values associated with this Component
     */
    Stats::ValueStatBlock getStats(std::string tag);

    void setResponseConnection(Connection<BankResponse>* connection);

    void setCommandConnection(Connection<BankCommand>* connection);

    bool busy() const;

    private:
    Connection<BankResponse>* responseConnection;
    Connection<BankCommand>* commandConnection;

    BankCommand currentCommand;
    std::unique_ptr<BankState> state;
    Stats::ValueStatBlock stats;
};

} // namespace NVM::ComponentType