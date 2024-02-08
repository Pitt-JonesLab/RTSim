#pragma once

#include "MemoryTypes/Component/BankCommand.h"
#include "MemoryTypes/Component/Component.h"
#include "MemoryTypes/Component/Connection.h"
#include "MemoryTypes/Component/MemoryCommand.h"

namespace NVM::ComponentType {

class MemoryController : public Component {
    public:
    MemoryController();

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

    bool issue(MemoryCommand command);

    void setCommandConnection(Connection<BankCommand>* connection);

    bool busy() const;

    private:
    Connection<BankCommand>* commandConnection;

    bool working;

    MemoryCommand currentCommand;
    BankCommand nextCommand;
};

} // namespace NVM::ComponentType