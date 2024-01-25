#include "MemoryTypes/Simple/Modeling/Decoder.h"

#include <catch2/catch_test_macros.hpp>

using namespace NVM::Modeling;

TEST_CASE("Decodes properly", "[Decoder], [Memory]") {
    ComponentCounts counts;
    counts.rows = 4;
    counts.channels = 4;
    counts.cols = 4;
    counts.ranks = 4;
    counts.banks = 4;

    setScheme("RK:BK:CH:R:C", counts);

    uint64_t testAddress = 0xC0;

    REQUIRE(decodeSymbol(AddressSymbol::COL, testAddress) == 3);
    testAddress <<= 2;
    REQUIRE(decodeSymbol(AddressSymbol::COL, testAddress) == 0);
    REQUIRE(decodeSymbol(AddressSymbol::ROW, testAddress) == 3);
    testAddress <<= 2;
    REQUIRE(decodeSymbol(AddressSymbol::ROW, testAddress) == 0);
    REQUIRE(decodeSymbol(AddressSymbol::CHANNEL, testAddress) == 3);
    testAddress <<= 2;
    REQUIRE(decodeSymbol(AddressSymbol::CHANNEL, testAddress) == 0);
    REQUIRE(decodeSymbol(AddressSymbol::BANK, testAddress) == 3);
    testAddress <<= 2;
    REQUIRE(decodeSymbol(AddressSymbol::BANK, testAddress) == 0);
    REQUIRE(decodeSymbol(AddressSymbol::RANK, testAddress) == 3);
    testAddress <<= 2;
    REQUIRE(decodeSymbol(AddressSymbol::RANK, testAddress) == 0);
}

TEST_CASE("Replaces properly", "[Decoder], [Memory]") {
    ComponentCounts counts;
    counts.rows = 4;
    counts.channels = 4;
    counts.cols = 4;
    counts.ranks = 4;
    counts.banks = 4;

    setScheme("RK:BK:CH:R:C", counts);

    uint64_t testAddress = 0x0;

    REQUIRE(decodeSymbol(AddressSymbol::BANK, testAddress) == 0);
    testAddress = replaceSymbol(testAddress, AddressSymbol::BANK, 1);
    REQUIRE(decodeSymbol(AddressSymbol::BANK, testAddress) == 1);

    REQUIRE(decodeSymbol(AddressSymbol::RANK, testAddress) == 0);
    testAddress = replaceSymbol(testAddress, AddressSymbol::RANK, 1);
    REQUIRE(decodeSymbol(AddressSymbol::RANK, testAddress) == 1);

    REQUIRE(decodeSymbol(AddressSymbol::ROW, testAddress) == 0);
    testAddress = replaceSymbol(testAddress, AddressSymbol::ROW, 1);
    REQUIRE(decodeSymbol(AddressSymbol::ROW, testAddress) == 1);

    REQUIRE(decodeSymbol(AddressSymbol::COL, testAddress) == 0);
    testAddress = replaceSymbol(testAddress, AddressSymbol::COL, 1);
    REQUIRE(decodeSymbol(AddressSymbol::COL, testAddress) == 1);

    REQUIRE(decodeSymbol(AddressSymbol::CHANNEL, testAddress) == 0);
    testAddress = replaceSymbol(testAddress, AddressSymbol::CHANNEL, 1);
    REQUIRE(decodeSymbol(AddressSymbol::CHANNEL, testAddress) == 1);
}