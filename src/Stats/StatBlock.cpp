#include "Stats/StatBlock.h"

#include "Logging/Logging.h"

#include <algorithm>

using namespace NVM::Stats;

std::ostream& NVM::Stats::operator<<(std::ostream& out,
                                     const StatValue& statValue) {
    out << '.' << statValue.name << ' ' << statValue.value->print();
    if (!statValue.unit.empty()) out << ' ' << statValue.unit;
    out << '\n';
    return out;
}

std::ostream& NVM::Stats::operator<<(std::ostream& out,
                                     const ValueStatValue& statValue) {
    out << '.' << statValue.name << ' ' << statValue.value->print();
    if (!statValue.unit.empty()) out << ' ' << statValue.unit;
    out << '\n';
    return out;
}

StatBlock::StatBlock(std::string t) : tag(t) {}

void StatBlock::addChild(StatBlock childBlock) {
    children.push_back(childBlock);
}

void StatBlock::addChildStat(StatBlock childBlock, std::string name,
                             std::string unit) {
    auto nameMatches = [&name](const StatValue& val) {
        return val.getName() == name;
    };

    auto childStat = std::find_if(childBlock.values.begin(),
                                  childBlock.values.end(), nameMatches);
    auto parentStat = std::find_if(values.begin(), values.end(), nameMatches);

    if (childStat == childBlock.values.end()) return;

    if (parentStat == values.end()) {
        values.push_back(*childStat);
        return;
    }
}

void StatBlock::log() const {
    for (const auto& val : values) {
        NVM::Logging::log() << NVM::Logging::LogLevel::STAT << tag << val;
    }
    for (const auto& child : children) child.log();
}

void StatBlock::log(std::ostream& out) const {
    for (const auto& val : values) {
        out << tag << val;
    }
    for (const auto& child : children) child.log();
}

NVM::Stats::ValueStatBlock::ValueStatBlock(std::string t) : tag(t) {}

void NVM::Stats::ValueStatBlock::addChildStat(ValueStatBlock childBlock,
                                              std::string name,
                                              std::string unit) {
    auto nameMatches = [&name](const ValueStatValue& val) {
        return val.getName() == name;
    };

    auto childStat = std::find_if(childBlock.values.begin(),
                                  childBlock.values.end(), nameMatches);
    auto parentStat = std::find_if(values.begin(), values.end(), nameMatches);

    if (childStat == childBlock.values.end()) return;

    if (parentStat == values.end()) {
        values.push_back(*childStat);
    } else {
        (*parentStat) = (*parentStat) + (*childStat);
    }
}

void NVM::Stats::ValueStatBlock::log() const {
    for (const auto& val : values) {
        NVM::Logging::log() << NVM::Logging::LogLevel::STAT << tag << val;
    }
    for (const auto& child : children) {
        child.log();
    }
}

void NVM::Stats::ValueStatBlock::log(std::ostream& out) const {
    for (const auto& val : values) {
        out << tag << val;
    }
    for (const auto& child : children) {
        child.log(out);
    }
}

ValueStatBlock& NVM::Stats::ValueStatBlock::operator+=(ValueStatBlock& rhs) {
    for (const auto& val : rhs.values)
        addChildStat(rhs, val.getName(), val.getUnit());
    return *this;
}

void NVM::Stats::ValueStatBlock::addChild(ValueStatBlock childBlock) {
    children.push_back(childBlock);
}
