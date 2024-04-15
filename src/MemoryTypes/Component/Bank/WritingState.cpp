#include "MemoryTypes/Component/Bank/WritingState.h"

#include "MemoryTypes/Component/Bank/OpenState.h"
#include "Stats/StatBlock.h"

using namespace NVM::ComponentType;

NVM::ComponentType::WritingState::WritingState(BankInfo& i, unsigned int r) :
    State(i),
    row(r),
    delay(1) {
    info.name = BankInfo::Name::WRITING;
}

void NVM::ComponentType::WritingState::process() {
    if (!delay) nextState = std::make_unique<OpenState>(info, row);
}

void NVM::ComponentType::WritingState::cycle() { delay--; }

bool NVM::ComponentType::WritingState::busy() const { return true; }

NVM::Stats::ValueStatBlock NVM::ComponentType::WritingState::getStats() {
    Stats::ValueStatBlock stats;
    stats.addStat(1, "writes");
    stats.addStat(info.energyCosts.writeEnergy, "write_energy");
    stats.addStat(info.energyCosts.writeEnergy, "total_energy");
    return stats;
}
