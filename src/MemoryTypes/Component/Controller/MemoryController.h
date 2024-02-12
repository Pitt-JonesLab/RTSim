#pragma once

#include "MemoryTypes/Component/Bank/BankCommand.h"
#include "MemoryTypes/Component/Component.h"
#include "MemoryTypes/Component/Connection.h"
#include "MemoryTypes/Component/Controller/BankModel.h"
#include "MemoryTypes/Component/Controller/Parser.h"
#include "MemoryTypes/Component/Controller/Scheduler.h"
#include "MemoryTypes/Component/System/MemoryCommand.h"

#include <queue>

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
    Stats::ValueStatBlock getStats(std::string tag);

    bool issue(MemoryCommand command);

    void setCommandConnection(Connection<BankCommand>* connection);

    bool busy() const;

    private:
    Connection<BankCommand>* commandConnection;

    bool issued;
    std::queue<BankCommand> bankQueue;

    Parser parser;
    Scheduler scheduler;
    BankModel bankModel;
    MemoryCommand nextCommand;
};

} // namespace NVM::ComponentType