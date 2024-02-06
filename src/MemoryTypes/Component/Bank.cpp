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
