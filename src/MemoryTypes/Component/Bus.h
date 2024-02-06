#pragma once

#include "MemoryTypes/Component/BankResponse.h"
#include "MemoryTypes/Component/Component.h"
#include "MemoryTypes/Component/Connection.h"
#include "MemoryTypes/Component/MemoryCommand.h"

namespace NVM::ComponentType {

class Bus : public Component {
    public:
    Bus();

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

    bool busy() const;

    Connection<BankResponse>* getResponseConnection();
    Connection<MemoryCommand>* getCommandConnection();

    private:
    Connection<BankResponse> bankConnection;
    Connection<MemoryCommand> commandConnection;
};

} // namespace NVM::ComponentType