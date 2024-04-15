#pragma once

#include "MemoryTypes/Component/Bank/BankCommand.h"
#include "MemoryTypes/Component/Bank/BankResponse.h"
#include "MemoryTypes/Component/Component.h"
#include "MemoryTypes/Component/Connection.h"

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
    Stats::ValueStatBlock getStats(std::string tag);

    bool busy() const;

    Connection<BankResponse>* getResponseConnection();
    Connection<BankCommand>* getCommandConnection();

    private:
    Connection<BankResponse> bankConnection;
    Connection<BankCommand> commandConnection;
};

} // namespace NVM::ComponentType