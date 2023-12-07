#include "Modeling/System/SimpleSystem.h"

#include "Modeling/Decoder.h"
#include "Modeling/Rank.h"

using namespace NVM::Modeling;

SimpleSystem::SimpleSystem() {}

void SimpleSystem::addRank(std::unique_ptr<Rank> rank) {
    ranks.emplace_back(std::move(rank));
}

RowNumber SimpleSystem::getOpenRow(Address address) const {
    return decodeRank(address).getOpenRow(address);
}

bool SimpleSystem::isOpen(Address address) const {
    return decodeRank(address).isOpen(address);
}

void SimpleSystem::closeRow(Address address) {
    decodeRank(address).closeRow(address);
}

void SimpleSystem::openRow(Address address) {
    decodeRank(address).openRow(address);
}

Rank& SimpleSystem::decodeRank(Address address) {
    auto rankNum = decodeSymbol(AddressSymbol::RANK, address);
    return *ranks[rankNum];
}

const Rank& SimpleSystem::decodeRank(Address address) const {
    auto rankNum = decodeSymbol(AddressSymbol::RANK, address);
    return *ranks[rankNum];
}