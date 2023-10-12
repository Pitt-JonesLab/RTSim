#include "Memory/Interconnect/SimpleInterconnect.h"

#include "Logging/Logging.h"
#include "MockComponent.h"

#include <catch2/catch_test_macros.hpp>
#include <sstream>

using namespace NVM::Memory;
using namespace NVM::Logging;

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

TEST_CASE("Logs events", "[SimpleInterconnect], [Memory]") {
    auto rank = std::unique_ptr<Rank>(new MockRank());
    SimpleInterconnect interconnect;
    interconnect.addRank(std::move(rank));
    std::stringstream logString;
    setLogOutput(logString);
    setLogLevel(LogLevel::EVENT);

    SECTION("Logs read") {
        REQUIRE(interconnect.read(0, {}));
        REQUIRE_FALSE(logString.str().empty());
    }

    SECTION("Logs write") {
        REQUIRE(interconnect.write(0, {}));
        REQUIRE_FALSE(logString.str().empty());
    }
}