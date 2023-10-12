#include "Memory/Rank/SimpleRank.h"

#include "MockComponent.h"

#include <catch2/catch_test_macros.hpp>

using namespace NVM::Memory;

TEST_CASE("Constructs", "[SimpleRank], [Memory]") {
    REQUIRE_NOTHROW(SimpleRank());
}

TEST_CASE("Sends requests to Bank", "[SimpleRank], [Memory]") {
    auto bankPtr = new MockBank();
    auto bank = std::unique_ptr<Bank>(bankPtr);

    SimpleRank rank;
    rank.addBank(std::move(bank));

    SECTION("Bank gets write") {
        REQUIRE(rank.write(0, {}));
        REQUIRE(bankPtr->writeFlag);
    }

    SECTION("Bank gets read") {
        REQUIRE(rank.read(0, {}));
        REQUIRE(bankPtr->readFlag);
    }
}