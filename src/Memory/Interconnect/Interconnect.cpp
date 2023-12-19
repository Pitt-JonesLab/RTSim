#include "Memory/Interconnect/Interconnect.h"

using namespace NVM::Memory;

bool Interconnect::issue(NVM::Command cmd) { return ranks[0]->issue(cmd); }

void Interconnect::addRank(std::unique_ptr<Rank> rank) {
    ranks.emplace_back(std::move(rank));
}

NVM::Stats::StatBlock Interconnect::getStats(std::string tag) const {
    NVM::Stats::StatBlock stats(tag);

    for (int i = 0; i < ranks.size(); i++) {
        auto rankStats = ranks[i]->getStats(tag + ".rank" + std::to_string(i));
        stats.addChildStat(rankStats, "activate_energy", "nJ");
        stats.addChildStat(rankStats, "read_energy", "nJ");
        stats.addChildStat(rankStats, "write_energy", "nJ");
        stats.addChildStat(rankStats, "reads");
        stats.addChildStat(rankStats, "writes");
        stats.addChildStat(rankStats, "activates");
        stats.addChildStat(rankStats, "precharges");
        stats.addChildStat(rankStats, "row_clones");
        stats.addChildStat(rankStats, "transverse_reads");
        stats.addChildStat(rankStats, "transverse_writes");
        stats.addChild(rankStats);
    }

    return stats;
}