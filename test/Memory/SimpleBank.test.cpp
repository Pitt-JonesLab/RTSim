#include "Memory/Bank/SimpleBank.h"

#include "Logging/Logging.h"
#include "MockComponent.h"

#include <catch2/catch_test_macros.hpp>
#include <sstream>

using namespace NVM::Memory;
using namespace NVM::Logging;

TEST_CASE("Constructs", "[SimpleBank], [Memory]") {
    REQUIRE_NOTHROW(SimpleBank());
}

TEST_CASE("Sends requests to SubArray", "[SimpleBank], [Memory]") {
    auto subArrayPtr = new MockSubArray();
    auto subArray = std::unique_ptr<SubArray>(subArrayPtr);

    SimpleBank bank;
    bank.addSubArray(std::move(subArray));

    SECTION("SubArray gets write") {
        REQUIRE(bank.write(0, {}));
        REQUIRE(subArrayPtr->writeFlag);
    }

    SECTION("SubArray gets read") {
        REQUIRE(bank.read(0, {}));
        REQUIRE(subArrayPtr->readFlag);
    }
}

TEST_CASE("Handles requests properly", "[SimpleBank], [Memory]") {
    auto subArrayPtr = new MockSubArray();
    auto subArray = std::unique_ptr<SubArray>(subArrayPtr);

    SimpleBank bank;
    bank.addSubArray(std::move(subArray));

    SECTION("One read at a time") {
        REQUIRE(bank.read(0, {}));
        REQUIRE_FALSE(bank.read(0, {}));
        subArrayPtr->command.notify();
        bank.cycle(1);
        REQUIRE(bank.read(0, {}));
    }

    SECTION("One write at a time") {
        REQUIRE(bank.write(0, {}));
        REQUIRE_FALSE(bank.write(0, {}));
        subArrayPtr->command.notify();
        bank.cycle(1);
        REQUIRE(bank.write(0, {}));
    }
}

TEST_CASE("Logs events", "[SimpleBank], [Memory]") {
    auto subArray = std::unique_ptr<SubArray>(new MockSubArray());
    SimpleBank bank;
    bank.addSubArray(std::move(subArray));
    std::stringstream logString;
    setLogOutput(logString);
    setLogLevel(LogLevel::EVENT);

    SECTION("Logs read") {
        REQUIRE(bank.read(0, {}));
        REQUIRE_FALSE(logString.str().empty());
    }

    SECTION("Logs write") {
        REQUIRE(bank.write(0, {}));
        REQUIRE_FALSE(logString.str().empty());
    }
}

TEST_CASE("Tracks stats", "[SimpleBank], [Memory]") {
    SimpleBank bank;
    std::stringstream logString;
    setLogOutput(logString);
    setLogLevel(LogLevel::STAT);

    StatBlock stats = bank.getStats("bank");
    stats.log();

    SECTION("Tracks reads") {
        REQUIRE(logString.str().find("bank.reads 0\n") != std::string::npos);
        REQUIRE(bank.read(0, {}));
        logString.str(std::string());
        stats.log();
        REQUIRE(logString.str().find("bank.reads 1\n") != std::string::npos);
    }

    SECTION("Tracks writes") {
        REQUIRE(logString.str().find("bank.writes 0\n") != std::string::npos);
        REQUIRE(bank.write(0, {}));
        logString.str(std::string());
        stats.log();
        REQUIRE(logString.str().find("bank.writes 1\n") != std::string::npos);
    }
}