#pragma once

#include "MemoryTypes/Component/BankCommand.h"
#include "MemoryTypes/Component/BankResponse.h"
#include "MemoryTypes/Component/Component.h"
#include "MemoryTypes/Component/Connection.h"

namespace NVM::ComponentType {

class Bank : public Component {
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
    Stats::StatBlock getStats();

    void setResponseConnection(Connection<BankResponse>* connection);

    void setCommandConnection(Connection<BankCommand>* connection);

    bool busy() const;

    private:
    Connection<BankResponse>* responseConnection;
    Connection<BankCommand>* commandConnection;
};

} // namespace NVM::ComponentType