#include "Memory/SubArray/SimpleSubArray.h"

#include "Logging/Logging.h"

#include <catch2/catch_test_macros.hpp>
#include <sstream>

using namespace NVM::Memory;
using namespace NVM::Logging;

TEST_CASE("Constructs", "[SimpleSubArray], [Memory]") {
    REQUIRE_NOTHROW(SimpleSubArray(1));
}

TEST_CASE("Handles requests properly", "[SimpleSubArray], [Memory]") {
    SimpleSubArray subArray(1);

    SECTION("Reads when available") {
        REQUIRE(subArray.read(0, {}));
        REQUIRE_FALSE(subArray.read(0, {}));
        subArray.cycle(5);
        REQUIRE(subArray.read(0, {}));
    }

    SECTION("Writes when available") {
        REQUIRE(subArray.write(0, {}));
        REQUIRE_FALSE(subArray.write(0, {}));
        subArray.cycle(5);
        REQUIRE(subArray.write(0, {}));
    }
}

TEST_CASE("Logs events", "[SimpleSubArray], [Memory]") {
    SimpleSubArray subArray(1);
    std::stringstream logString;
    setLogOutput(logString);
    setLogLevel(LogLevel::EVENT);

    SECTION("Logs read") {
        REQUIRE(subArray.read(0, {}));
        REQUIRE_FALSE(logString.str().empty());
    }

    SECTION("Logs write") {
        REQUIRE(subArray.write(0, {}));
        REQUIRE_FALSE(logString.str().empty());
    }
}

TEST_CASE("Tracks stats", "[SimpleSubArray], [Memory]") {
    SimpleSubArray subArray(1);
    std::stringstream logString;
    setLogOutput(logString);
    setLogLevel(LogLevel::STAT);

    StatBlock stats = subArray.getStats("subArray");
    stats.log();

    SECTION("Tracks reads") {
        INFO("Stat output was:\n" + logString.str());
        REQUIRE(logString.str().find("subArray.reads 0\n") !=
                std::string::npos);
        REQUIRE(subArray.read(0, {}));
        logString.str(std::string());
        stats.log();
        INFO("Stat output was:\n" + logString.str());
        REQUIRE(logString.str().find("subArray.reads 1\n") !=
                std::string::npos);
    }

    SECTION("Tracks writes") {
        INFO("Stat output was:\n" + logString.str());
        REQUIRE(logString.str().find("subArray.writes 0\n") !=
                std::string::npos);
        REQUIRE(subArray.write(0, {}));
        logString.str(std::string());
        stats.log();
        INFO("Stat output was:\n" + logString.str());
        REQUIRE(logString.str().find("subArray.writes 1\n") !=
                std::string::npos);
    }
}