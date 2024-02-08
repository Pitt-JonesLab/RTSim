#include "MemoryTypes/Component/Bus.h"

#include "MemoryTypes/Component/SimpleBankTimingRules.h"
#include "Stats/StatBlock.h"

using namespace NVM::ComponentType;

NVM::ComponentType::Bus::Bus() {
    auto rules = std::make_unique<SimpleBankTimingRules>();
    commandConnection.setTimingRules(std::move(rules));
}

void NVM::ComponentType::Bus::process() {}

void NVM::ComponentType::Bus::cycle() {
    bankConnection.cycle();
    commandConnection.cycle();
}

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
