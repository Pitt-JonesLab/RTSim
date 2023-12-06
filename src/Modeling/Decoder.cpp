#include "Modeling/Decoder.h"

#include <cmath>
#include <iostream>
#include <map>
#include <sstream>
#include <vector>

using namespace NVM::Modeling;

std::map<AddressSymbol, std::pair<unsigned int, unsigned int>> symbolPositions;

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
        symbolPositions[symbol] = {lsb + bitLength, lsb};
        lsb += bitLength + 1;
    }
}

unsigned int getBitRange(uint64_t val, unsigned int msb, unsigned int lsb) {
    val >>= lsb;
    uint64_t mask = 1;
    for (int i = 0; i < (msb - lsb); i++) {
        mask <<= 1;
        mask++;
    }
    return val & mask;
}

unsigned int NVM::Modeling::decodeSymbol(AddressSymbol symbol,
                                         uint64_t address) {
    // Truncating bus offset and burst length TODO: find these numbers
    address >>= 6;
    if (!symbolPositions.count(symbol)) return 0;
    return getBitRange(address, symbolPositions[symbol].first,
                       symbolPositions[symbol].second);
}