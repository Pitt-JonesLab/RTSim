#include "MemoryTypes/Component/Bank/ReadingState.h"

#include "MemoryTypes/Component/Bank/OpenState.h"
#include "Stats/StatBlock.h"

using namespace NVM::ComponentType;

NVM::ComponentType::ReadingState::ReadingState(BankInfo& i, unsigned int r) :
    State(i),
    delay(1),
    row(r) {
    info.name = BankInfo::Name::READING;
}

void NVM::ComponentType::ReadingState::process() {
    if (!delay) nextState = std::make_unique<OpenState>(info, row);
}

void NVM::ComponentType::ReadingState::cycle() { delay--; }

bool NVM::ComponentType::ReadingState::busy() const { return true; }

NVM::Stats::ValueStatBlock NVM::ComponentType::ReadingState::getStats() {
    Stats::ValueStatBlock stats;
    stats.addStat(1, "reads");
    stats.addStat(info.energyCosts.readEnergy, "read_energy");
    stats.addStat(info.energyCosts.readEnergy, "total_energy");
    return stats;
}
