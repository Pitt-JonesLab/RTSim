#include "MemoryTypes/Component/Bank.h"

#include "Stats/StatBlock.h"

NVM::ComponentType::Bank::Bank() :
    responseConnection(nullptr),
    commandConnection(nullptr) {}

void NVM::ComponentType::Bank::process() {}

void NVM::ComponentType::Bank::cycle() {}

NVM::Stats::StatBlock NVM::ComponentType::Bank::getStats() {
    return Stats::StatBlock();
}

void NVM::ComponentType::Bank::setResponseConnection(
    Connection<BankResponse>* connection) {
    responseConnection = connection;
}

void NVM::ComponentType::Bank::setCommandConnection(
    Connection<MemoryCommand>* connection) {
    commandConnection = connection;
}

bool NVM::ComponentType::Bank::busy() const { return false; }
