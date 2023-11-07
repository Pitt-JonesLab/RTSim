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
    }

    SECTION("Rank gets read") {
        REQUIRE(interconnect.read(0, {}));
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
    }

    SECTION("One write at a time") {
        REQUIRE(interconnect.write(0, {}));
        REQUIRE_FALSE(interconnect.write(0, {}));
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

TEST_CASE("Tracks stats", "[SimpleInterconnect], [Memory]") {
    SimpleInterconnect interconnect;
    interconnect.addRank(std::unique_ptr<Rank>(new MockRank()));
    std::stringstream logString;
    setLogOutput(logString);
    setLogLevel(LogLevel::STAT);

    StatBlock stats = interconnect.getStats("interconnect");
    stats.log();

    SECTION("Tracks reads") {
        INFO("Stat output was:\n" + logString.str());
        REQUIRE(logString.str().find("interconnect.reads 0\n") !=
                std::string::npos);
        REQUIRE(interconnect.read(0, {}));
        logString.str(std::string());
        stats.log();
        INFO("Stat output was:\n" + logString.str());
        REQUIRE(logString.str().find("interconnect.reads 1\n") !=
                std::string::npos);
    }

    SECTION("Tracks writes") {
        INFO("Stat output was:\n" + logString.str());
        REQUIRE(logString.str().find("interconnect.writes 0\n") !=
                std::string::npos);
        REQUIRE(interconnect.write(0, {}));
        logString.str(std::string());
        stats.log();
        INFO("Stat output was:\n" + logString.str());
        REQUIRE(logString.str().find("interconnect.writes 1\n") !=
                std::string::npos);
    }
}