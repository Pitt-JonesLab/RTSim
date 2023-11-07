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
    }

    SECTION("Bank gets read") {
        REQUIRE(rank.read(0, {}));
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
    }

    SECTION("One write at a time") {
        REQUIRE(rank.write(0, {}));
        REQUIRE_FALSE(rank.write(0, {}));
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

TEST_CASE("Tracks stats", "[SimpleRank], [Memory]") {
    SimpleRank rank;
    rank.addBank(std::unique_ptr<Bank>(new MockBank()));
    std::stringstream logString;
    setLogOutput(logString);
    setLogLevel(LogLevel::STAT);

    StatBlock stats = rank.getStats("rank");
    stats.log();

    SECTION("Tracks reads") {
        INFO("Stat output was:\n" + logString.str());
        REQUIRE(logString.str().find("rank.reads 0\n") != std::string::npos);
        REQUIRE(rank.read(0, {}));
        logString.str(std::string());
        stats.log();
        INFO("Stat output was:\n" + logString.str());
        REQUIRE(logString.str().find("rank.reads 1\n") != std::string::npos);
    }

    SECTION("Tracks writes") {
        INFO("Stat output was:\n" + logString.str());
        REQUIRE(logString.str().find("rank.writes 0\n") != std::string::npos);
        REQUIRE(rank.write(0, {}));
        logString.str(std::string());
        stats.log();
        INFO("Stat output was:\n" + logString.str());
        REQUIRE(logString.str().find("rank.writes 1\n") != std::string::npos);
    }
}