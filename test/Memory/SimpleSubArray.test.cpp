#include "Memory/SubArray/SimpleSubArray.h"

#include "Logging/Logging.h"

#include <catch2/catch_test_macros.hpp>
#include <sstream>

using namespace NVM::Memory;
using namespace NVM::Logging;

TEST_CASE("Constructs", "[SimpleSubArray], [Memory]") {
    REQUIRE_NOTHROW(SimpleSubArray());
}

TEST_CASE("Handles requests properly", "[SimpleSubArray], [Memory]") {
    SimpleSubArray subArray;

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
    SimpleSubArray subArray;
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