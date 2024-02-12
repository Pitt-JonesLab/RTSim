#include "MemoryTypes/Component/Bank.h"

#include "Logging/Logging.h"
#include "MemoryTypes/Component/BankCommand.h"
#include "MemoryTypes/Component/ClosedState.h"
#include "Stats/StatBlock.h"

using namespace NVM::ComponentType;

NVM::ComponentType::Bank::Bank() :
    responseConnection(nullptr),
    commandConnection(nullptr),
    StateMachine(
        std::make_unique<ClosedState>(commandConnection, responseConnection)) {
    stats.addStat(0, "reads");
    stats.addStat(0, "writes");
    stats.addStat(0, "precharges");
    stats.addStat(0, "activates");
}

void NVM::ComponentType::Bank::setResponseConnection(
    Connection<BankResponse>* connection) {
    responseConnection = connection;
    state =
        std::make_unique<ClosedState>(commandConnection, responseConnection);
}

void NVM::ComponentType::Bank::setCommandConnection(
    Connection<BankCommand>* connection) {
    commandConnection = connection;
    state =
        std::make_unique<ClosedState>(commandConnection, responseConnection);
}

bool NVM::ComponentType::Bank::busy() const { return state->busy(); }
