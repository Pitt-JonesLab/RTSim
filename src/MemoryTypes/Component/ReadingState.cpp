#include "MemoryTypes/Component/ReadingState.h"

#include "MemoryTypes/Component/OpenState.h"
#include "Stats/StatBlock.h"

using namespace NVM::ComponentType;

NVM::ComponentType::ReadingState::ReadingState(
    Connection<BankCommand>* cmd, Connection<BankResponse>* response,
    unsigned int r) :
    BankState(cmd, response),
    delay(1),
    row(r) {}

void NVM::ComponentType::ReadingState::process() {
    if (!delay)
        nextState = std::make_unique<OpenState>(commandConnection,
                                                responseConnection, row);
}

void NVM::ComponentType::ReadingState::cycle() { delay--; }

bool NVM::ComponentType::ReadingState::busy() const { return true; }

NVM::Stats::ValueStatBlock NVM::ComponentType::ReadingState::getStats() {
    Stats::ValueStatBlock stats;
    stats.addStat(1, "reads");
    return stats;
}
