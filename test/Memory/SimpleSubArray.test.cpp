#include "Memory/SubArray/SimpleSubArray.h"

#include <catch2/catch_test_macros.hpp>

using namespace NVM::Memory;

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