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

NVM::Stats::ValueStatBlock NVM::ComponentType::Bus::getStats(std::string tag) {
    return Stats::ValueStatBlock(tag);
}

bool NVM::ComponentType::Bus::busy() const {
    return commandConnection.busy() || bankConnection.busy();
}

Connection<BankResponse>* NVM::ComponentType::Bus::getResponseConnection() {
    return &bankConnection;
}

Connection<BankCommand>* NVM::ComponentType::Bus::getCommandConnection() {
    return &commandConnection;
}
