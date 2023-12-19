#include "Memory/Rank/SimpleRank.h"

using namespace NVM::Memory;

void SimpleRank::cycle(unsigned int cycles) {
    if (!banks.empty()) banks[0]->cycle(cycles);
}

bool SimpleRank::isEmpty() const {
    if (banks.empty()) return false;
    return banks[0]->isEmpty();
}

NVM::Stats::StatBlock SimpleRank::getStats(std::string tag) const {
    NVM::Stats::StatBlock stats(tag);

    for (int i = 0; i < banks.size(); i++) {
        auto bankStats = banks[i]->getStats(tag + ".bank" + std::to_string(i));
        stats.addChildStat(bankStats, "activate_energy", "nJ");
        stats.addChildStat(bankStats, "read_energy", "nJ");
        stats.addChildStat(bankStats, "write_energy", "nJ");
        stats.addChildStat(bankStats, "reads");
        stats.addChildStat(bankStats, "writes");
        stats.addChildStat(bankStats, "activates");
        stats.addChildStat(bankStats, "precharges");
        stats.addChildStat(bankStats, "row_clones");
        stats.addChildStat(bankStats, "transverse_reads");
        stats.addChildStat(bankStats, "transverse_writes");
        stats.addChild(bankStats);
    }

    return stats;
}