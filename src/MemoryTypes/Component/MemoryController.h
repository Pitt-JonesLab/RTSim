#pragma once

#include "MemoryTypes/Component/BankCommand.h"
#include "MemoryTypes/Component/BankModel.h"
#include "MemoryTypes/Component/Component.h"
#include "MemoryTypes/Component/Connection.h"
#include "MemoryTypes/Component/MemoryCommand.h"
#include "MemoryTypes/Component/Parser.h"
#include "MemoryTypes/Component/Scheduler.h"

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