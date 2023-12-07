#include "Modeling/Rank/SimpleRank.h"

#include "Bank/MockBank.h"
#include "Modeling/Decoder.h"

#include <catch2/catch_test_macros.hpp>

using namespace NVM::Modeling;

TEST_CASE("Works properly", "[SimpleRank], [Modeling]") {
    ComponentCounts counts;
    counts.banks = 2;
    counts.ranks = 1;
    counts.channels = 1;
    counts.cols = 256;
    counts.rows = 256;
    setScheme("RK:BK:R:C:CH", counts);

    SimpleRank rank;
    Address address = 0;

    MockBank* bank0 = new MockBank();
    MockBank* bank1 = new MockBank();

    rank.addBank(std::unique_ptr<Bank>(bank0));
    rank.addBank(std::unique_ptr<Bank>(bank1));

    SECTION("isOpen") {
        // Bank 0
        REQUIRE_FALSE(rank.isOpen(address));
        rank.openRow(address);
        REQUIRE(rank.isOpen(address));
        rank.closeRow(address);
        REQUIRE_FALSE(rank.isOpen(address));

        // Bank 1
        address = replaceSymbol(address, AddressSymbol::BANK, 1);
        REQUIRE_FALSE(rank.isOpen(address));
        rank.openRow(address);
        REQUIRE(rank.isOpen(address));
        rank.closeRow(address);
        REQUIRE_FALSE(rank.isOpen(address));
    }

    SECTION("openRow") {
        // Bank 0
        rank.openRow(address);
        REQUIRE(rank.getOpenRow(address) == 0);
        rank.closeRow(address);
        address = replaceSymbol(address, AddressSymbol::ROW, 1);
        rank.openRow(address);
        REQUIRE(rank.getOpenRow(address) == 1);

        // Bank 1
        address = 0;
        address = replaceSymbol(address, AddressSymbol::BANK, 1);
        INFO("Bank is " << decodeSymbol(AddressSymbol::BANK, address)
                        << ", Row is "
                        << decodeSymbol(AddressSymbol::ROW, address));
        rank.openRow(address);
        REQUIRE(rank.getOpenRow(address) == 0);
        rank.closeRow(address);
        address = replaceSymbol(address, AddressSymbol::ROW, 1);
        rank.openRow(address);
        REQUIRE(rank.getOpenRow(address) == 1);
    }
}