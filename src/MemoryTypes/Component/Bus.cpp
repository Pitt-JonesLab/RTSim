#include "MemoryTypes/Component/Bus.h"

#include "Stats/StatBlock.h"

using namespace NVM::ComponentType;

NVM::ComponentType::Bus::Bus() {}

void NVM::ComponentType::Bus::process() {}

void NVM::ComponentType::Bus::cycle() {}

NVM::Stats::StatBlock NVM::ComponentType::Bus::getStats() {
    return Stats::StatBlock();
}

bool NVM::ComponentType::Bus::busy() const { return false; }

Connection<BankResponse>* NVM::ComponentType::Bus::getResponseConnection() {
    return &bankConnection;
}

Connection<BankCommand>* NVM::ComponentType::Bus::getCommandConnection() {
    return &commandConnection;
}
