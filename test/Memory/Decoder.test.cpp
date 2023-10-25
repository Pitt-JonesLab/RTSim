#include "Memory/Decoder.h"

#include <catch2/catch_test_macros.hpp>

using namespace NVM::Memory;

TEST_CASE("Decodes properly", "[Decoder], [Memory]") {
    Decoder::ComponentCounts counts;
    counts.rows = 2;
    counts.channels = 2;
    counts.cols = 2;
    counts.ranks = 2;
    counts.banks = 2;

    Decoder::setScheme("RK:BK:CH:R:C", counts);

    uint64_t testAddress = 0x3;

    REQUIRE(Decoder::decodeSymbol(Decoder::AddressSymbol::COL, testAddress) == 3);
    testAddress <<= 2;
    REQUIRE(Decoder::decodeSymbol(Decoder::AddressSymbol::COL, testAddress) == 0);
    REQUIRE(Decoder::decodeSymbol(Decoder::AddressSymbol::ROW, testAddress) == 3);
    testAddress <<= 2;
    REQUIRE(Decoder::decodeSymbol(Decoder::AddressSymbol::ROW, testAddress) == 0);
    REQUIRE(Decoder::decodeSymbol(Decoder::AddressSymbol::CHANNEL, testAddress) == 3);
    testAddress <<= 2;
    REQUIRE(Decoder::decodeSymbol(Decoder::AddressSymbol::CHANNEL, testAddress) == 0);
    REQUIRE(Decoder::decodeSymbol(Decoder::AddressSymbol::BANK, testAddress) == 3);
    testAddress <<= 2;
    REQUIRE(Decoder::decodeSymbol(Decoder::AddressSymbol::BANK, testAddress) == 0);
    REQUIRE(Decoder::decodeSymbol(Decoder::AddressSymbol::RANK, testAddress) == 3);
    testAddress <<= 2;
    REQUIRE(Decoder::decodeSymbol(Decoder::AddressSymbol::RANK, testAddress) == 0);
}