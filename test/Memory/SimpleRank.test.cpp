#include "Memory/Rank/SimpleRank.h"

#include "Logging/Logging.h"
#include "MockComponent.h"

#include <catch2/catch_test_macros.hpp>
#include <sstream>

using namespace NVM::Memory;
using namespace NVM::Logging;

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

TEST_CASE("Handles requests properly", "[SimpleRank], [Memory]") {
    auto bankPtr = new MockBank();
    auto bank = std::unique_ptr<Bank>(bankPtr);

    SimpleRank rank;
    rank.addBank(std::move(bank));

    SECTION("One read at a time") {
        REQUIRE(rank.read(0, {}));
        REQUIRE_FALSE(rank.read(0, {}));
        bankPtr->command.notify();
        rank.cycle(1);
        REQUIRE(rank.read(0, {}));
    }

    SECTION("One write at a time") {
        REQUIRE(rank.write(0, {}));
        REQUIRE_FALSE(rank.write(0, {}));
        bankPtr->command.notify();
        rank.cycle(1);
        REQUIRE(rank.write(0, {}));
    }
}

TEST_CASE("Logs events", "[SimpleRank], [Memory]") {
    auto bank = std::unique_ptr<Bank>(new MockBank());
    SimpleRank rank;
    rank.addBank(std::move(bank));
    std::stringstream logString;
    setLogOutput(logString);
    setLogLevel(LogLevel::EVENT);

    SECTION("Logs read") {
        REQUIRE(rank.read(0, {}));
        REQUIRE_FALSE(logString.str().empty());
    }

    SECTION("Logs write") {
        REQUIRE(rank.write(0, {}));
        REQUIRE_FALSE(logString.str().empty());
    }
}