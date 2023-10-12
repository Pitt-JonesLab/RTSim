#include "Memory/Interconnect/SimpleInterconnect.h"

#include "MockComponent.h"

#include <catch2/catch_test_macros.hpp>

using namespace NVM::Memory;

TEST_CASE("Constructs", "[SimpleInterconnect], [Memory]") {
    REQUIRE_NOTHROW(SimpleInterconnect());
}

TEST_CASE("Sends requests to Rank", "[SimpleInterconnect], [Memory]") {
    auto rankPtr = new MockRank();
    auto rank = std::unique_ptr<Rank>(rankPtr);

    SimpleInterconnect interconnect;
    interconnect.addRank(std::move(rank));

    SECTION("Rank gets write") {
        REQUIRE(interconnect.write(0, {}));
        REQUIRE(rankPtr->writeFlag);
    }

    SECTION("Rank gets read") {
        REQUIRE(interconnect.read(0, {}));
        REQUIRE(rankPtr->readFlag);
    }
}

TEST_CASE("Handles requests properly", "[SimpleInterconnect], [Memory]") {
    auto rankPtr = new MockRank();
    auto rank = std::unique_ptr<Rank>(rankPtr);

    SimpleInterconnect interconnect;
    interconnect.addRank(std::move(rank));

    SECTION("One read at a time") {
        REQUIRE(interconnect.read(0, {}));
        REQUIRE_FALSE(interconnect.read(0, {}));
        rankPtr->command.notify();
        interconnect.cycle(1);
        REQUIRE(interconnect.read(0, {}));
    }

    SECTION("One write at a time") {
        REQUIRE(interconnect.write(0, {}));
        REQUIRE_FALSE(interconnect.write(0, {}));
        rankPtr->command.notify();
        interconnect.cycle(1);
        REQUIRE(interconnect.write(0, {}));
    }
}