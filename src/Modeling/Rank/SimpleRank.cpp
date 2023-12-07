#include "Modeling/Rank/SimpleRank.h"

#include "Modeling/Bank.h"
#include "Modeling/Decoder.h"

using namespace NVM::Modeling;

SimpleRank::SimpleRank() {}

void SimpleRank::addBank(std::unique_ptr<Bank> bank) {
    banks.emplace_back(std::move(bank));
}

RowNumber SimpleRank::getOpenRow(Address address) const {
    return decodeBank(address).getOpenRow(address);
}

bool SimpleRank::isOpen(Address address) const {
    return decodeBank(address).isOpen(address);
}

void SimpleRank::closeRow(Address address) {
    decodeBank(address).closeRow(address);
}

void SimpleRank::openRow(Address address) {
    decodeBank(address).openRow(address);
}

Bank& SimpleRank::decodeBank(Address address) {
    auto bankNum = decodeSymbol(AddressSymbol::BANK, address);
    return *banks[bankNum];
}

const Bank& SimpleRank::decodeBank(Address address) const {
    auto bankNum = decodeSymbol(AddressSymbol::BANK, address);
    return *banks[bankNum];
}