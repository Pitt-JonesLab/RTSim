#include "MemoryTypes/Component/Bank.h"

#include "Logging/Logging.h"
#include "MemoryTypes/Component/BankCommand.h"
#include "MemoryTypes/Component/ClosedState.h"
#include "Stats/StatBlock.h"

using namespace NVM::ComponentType;

NVM::ComponentType::Bank::Bank() :
    responseConnection(nullptr),
    commandConnection(nullptr),
    currentCommand(BankCommand::Opcode::NO_OP),
    state(nullptr) {}

void NVM::ComponentType::Bank::process() { state->process(); }

void NVM::ComponentType::Bank::cycle() { state->cycle(); }

NVM::Stats::StatBlock NVM::ComponentType::Bank::getStats() {
    return Stats::StatBlock();
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

bool NVM::ComponentType::Bank::busy() const { return false; }
