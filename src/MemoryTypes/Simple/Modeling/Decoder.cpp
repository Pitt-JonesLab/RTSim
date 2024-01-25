#include "MemoryTypes/Simple/Modeling/Decoder.h"

#include <cmath>
#include <iostream>
#include <map>
#include <sstream>
#include <vector>

using namespace NVM::Modeling;

std::array<std::pair<unsigned int, unsigned int>, 5> symbolPositions;

AddressSymbol stringToSymbol(std::string token) {
    if (token == "R") return AddressSymbol::ROW;
    if (token == "CH") return AddressSymbol::CHANNEL;
    if (token == "RK") return AddressSymbol::RANK;
    if (token == "BK") return AddressSymbol::BANK;
    if (token == "C") return AddressSymbol::COL;
    // TODO: Return some invalid state instead of COL
    return AddressSymbol::COL;
}

unsigned int minBits(unsigned int count) {
    count--;
    unsigned int bits = 0;
    while (count >>= 1) bits++;
    return bits;
}

unsigned int getBitLength(AddressSymbol symbol, ComponentCounts counts) {
    switch (symbol) {
        case AddressSymbol::ROW:
            return minBits(counts.rows);
        case AddressSymbol::COL:
            return minBits(counts.cols);
        case AddressSymbol::CHANNEL:
            return minBits(counts.channels);
        case AddressSymbol::RANK:
            return minBits(counts.ranks);
        case AddressSymbol::BANK:
            return minBits(counts.banks);
    }
    return 0;
}

std::vector<std::string> splitTokens(std::string order) {
    std::stringstream ss(order);
    std::string token;
    std::vector<std::string> tokens;
    while (!getline(ss, token, ':').eof()) tokens.push_back(token);
    tokens.push_back(token);
    return std::vector<std::string>(tokens.rbegin(), tokens.rend());
}

void NVM::Modeling::setScheme(std::string order, ComponentCounts counts) {
    unsigned int lsb = 0;
    for (auto token : splitTokens(order)) {
        auto symbol = stringToSymbol(token);
        auto bitLength = getBitLength(symbol, counts);
        symbolPositions[static_cast<size_t>(symbol)] = {lsb + bitLength, lsb};
        lsb += bitLength + 1;
    }
}

/**
 * Get a range of bits from a value, starting at the lsb and ending at the msb
 * @param val the value to get the bits from.
 * @param msb the most significant bit to get.
 * @param lsb the least significant bit to get.
 */
unsigned int getBitRange(uint64_t val, unsigned int msb, unsigned int lsb) {
    uint64_t mask = 0xFFFFFFFFFFFFFFFF;
    mask >>= 64 - (msb - lsb + 1);
    return (val >> lsb) & mask;
}

/**
 * Decode a symbol from an address.
 * @param symbol the symbol to decode.
 * @param address the address to decode from.
 */
unsigned int NVM::Modeling::decodeSymbol(AddressSymbol symbol,
                                         uint64_t address) {
    // Truncating bus offset and burst length TODO: find these numbers
    address >>= 6;
    return getBitRange(address,
                       symbolPositions[static_cast<size_t>(symbol)].first,
                       symbolPositions[static_cast<size_t>(symbol)].second);
}

Address NVM::Modeling::replaceSymbol(Address address, AddressSymbol symbol,
                                     unsigned int newVal) {
    auto symbolLength = symbolPositions[static_cast<size_t>(symbol)].first -
                        symbolPositions[static_cast<size_t>(symbol)].second;
    Address mask = 0;
    for (int i = 0; i < symbolLength; i++) {
        mask |= 1;
        mask <<= 1;
    }
    mask <<= symbolPositions[static_cast<size_t>(symbol)].second + 6;
    address &= ~mask;
    address |=
        (newVal << (symbolPositions[static_cast<size_t>(symbol)].second + 6));
    return address;
}