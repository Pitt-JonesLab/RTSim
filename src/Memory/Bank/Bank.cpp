#include "Memory/Bank/Bank.h"

using namespace NVM::Memory;

void Bank::addSubArray(std::unique_ptr<SubArray> subArray) {
    subArrays.emplace_back(std::move(subArray));
}

bool Bank::issue(NVM::Command cmd) { return subArrays[0]->issue(cmd); }

NVM::Stats::StatBlock Bank::getStats(std::string tag) const {
    NVM::Stats::StatBlock stats(tag);

    for (int i = 0; i < subArrays.size(); i++) {
        auto subArrayStats =
            subArrays[i]->getStats(tag + ".subArray" + std::to_string(i));
        stats.addChildStat(subArrayStats, "activate_energy", "nJ");
        stats.addChildStat(subArrayStats, "read_energy", "nJ");
        stats.addChildStat(subArrayStats, "write_energy", "nJ");
        stats.addChildStat(subArrayStats, "reads");
        stats.addChildStat(subArrayStats, "writes");
        stats.addChildStat(subArrayStats, "activates");
        stats.addChildStat(subArrayStats, "precharges");
        stats.addChildStat(subArrayStats, "row_clones");
        stats.addChildStat(subArrayStats, "transverse_reads");
        stats.addChildStat(subArrayStats, "transverse_writes");
        stats.addChild(subArrayStats);
    }

    return stats;
}