#include "MemoryTypes/Component/WritingState.h"

#include "MemoryTypes/Component/OpenState.h"
#include "Stats/StatBlock.h"

using namespace NVM::ComponentType;

NVM::ComponentType::WritingState::WritingState(
    Connection<BankCommand>* cmd, Connection<BankResponse>* response,
    unsigned int r) :
    BankState(cmd, response),
    row(r),
    delay(1) {}

void NVM::ComponentType::WritingState::process() {
    if (!delay)
        next = std::make_unique<OpenState>(commandConnection,
                                           responseConnection, row);
}

void NVM::ComponentType::WritingState::cycle() { delay--; }

bool NVM::ComponentType::WritingState::busy() const { return true; }

std::unique_ptr<BankState> NVM::ComponentType::WritingState::nextState() {
    return std::move(next);
}

NVM::Stats::ValueStatBlock NVM::ComponentType::WritingState::getStats() {
    Stats::ValueStatBlock stats;
    stats.addStat(1, "writes");
    return stats;
}
