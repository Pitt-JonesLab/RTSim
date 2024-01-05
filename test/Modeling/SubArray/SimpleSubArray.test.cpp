#include "Modeling/SubArray/SimpleSubArray.h"

#include <catch2/catch_test_macros.hpp>

using namespace NVM::Modeling;

TEST_CASE("Works properly", "[SimpleSubArray], [Modeling]") {
    SimpleSubArray subArray;

    SECTION("isOpen") {
        REQUIRE_FALSE(subArray.isOpen());
        subArray.openRow(0);
        REQUIRE(subArray.isOpen());
        subArray.closeRow();
        REQUIRE_FALSE(subArray.isOpen());
    }

    SECTION("openRow") {
        REQUIRE_THROWS(subArray.getOpenRow());
        subArray.openRow(0);
        REQUIRE(subArray.getOpenRow() == 0);
        REQUIRE_THROWS(subArray.openRow(1));
        subArray.closeRow();
        REQUIRE_THROWS(subArray.getOpenRow());
        subArray.openRow(1);
        REQUIRE(subArray.getOpenRow() == 1);
    }
}