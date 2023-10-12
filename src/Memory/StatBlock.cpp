#include "Memory/StatBlock.h"

using namespace NVM::Memory;

std::ostream& NVM::Memory::operator<<(std::ostream& out,
                                      const StatValue& statValue) {
    out << statValue.value->print();
    if (!statValue.unit.empty()) out << ' ' << statValue.unit;
    out << '\n';
    return out;
}

StatBlock::StatBlock(std::string t) : tag(t) {}

void StatBlock::addChild(StatBlock childBlock) {
    children.push_back(childBlock);
}

void StatBlock::log() const {
    for (const auto& val : values) {
        NVM::Logging::log() << NVM::Logging::LogLevel::STAT << val;
    }
    for (const auto& child : children) child.log();
}

void StatBlock::log(std::ostream& out) const {
    for (const auto& val : values) {
        out << val;
    }
    for (const auto& child : children) child.log();
}