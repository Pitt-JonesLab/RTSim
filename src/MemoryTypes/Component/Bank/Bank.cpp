#include "MemoryTypes/Component/Bank/Bank.h"

#include "Logging/Logging.h"
#include "MemoryTypes/Component/Bank/BankCommand.h"
#include "MemoryTypes/Component/Bank/ClosedState.h"
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
    stats.addStat(0, "row_clones");
    stats.addStat(0, "transverse_reads");
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
