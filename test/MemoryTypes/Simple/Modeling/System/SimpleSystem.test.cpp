#include "MemoryTypes/Simple/Modeling/System/SimpleSystem.h"

#include "MemoryTypes/Simple/Modeling/Decoder.h"
#include "MemoryTypes/Simple/Modeling/Rank/MockRank.h"

#include <catch2/catch_test_macros.hpp>

using namespace NVM::Modeling;

TEST_CASE("Works properly", "[SimpleSystem], [Modeling]") {
    ComponentCounts counts;
    counts.ranks = 2;
    counts.banks = 1;
    counts.channels = 1;
    counts.cols = 256;
    counts.rows = 256;
    setScheme("RK:BK:R:C:CH", counts);

    SimpleSystem system;
    Address address = 0;

    MockRank* rank0 = new MockRank();
    MockRank* rank1 = new MockRank();

    system.addRank(std::unique_ptr<Rank>(rank0));
    system.addRank(std::unique_ptr<Rank>(rank1));

    SECTION("isOpen") {
        // rank 0
        REQUIRE_FALSE(system.isOpen(address));
        system.openRow(address);
        REQUIRE(system.isOpen(address));
        system.closeRow(address);
        REQUIRE_FALSE(system.isOpen(address));

        // rank 1
        address = replaceSymbol(address, AddressSymbol::RANK, 1);
        REQUIRE_FALSE(system.isOpen(address));
        system.openRow(address);
        REQUIRE(system.isOpen(address));
        system.closeRow(address);
        REQUIRE_FALSE(system.isOpen(address));
    }

    SECTION("openRow") {
        // rank 0
        system.openRow(address);
        REQUIRE(system.getOpenRow(address) == 0);
        system.closeRow(address);
        address = replaceSymbol(address, AddressSymbol::ROW, 1);
        system.openRow(address);
        REQUIRE(system.getOpenRow(address) == 1);

        // rank 1
        address = 0;
        address = replaceSymbol(address, AddressSymbol::RANK, 1);
        INFO("rank is " << decodeSymbol(AddressSymbol::RANK, address)
                        << ", Row is "
                        << decodeSymbol(AddressSymbol::ROW, address));
        system.openRow(address);
        REQUIRE(system.getOpenRow(address) == 0);
        system.closeRow(address);
        address = replaceSymbol(address, AddressSymbol::ROW, 1);
        system.openRow(address);
        REQUIRE(system.getOpenRow(address) == 1);
    }
}