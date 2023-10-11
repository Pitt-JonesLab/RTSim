#include "Memory/SubArray/SimpleSubArray.h"

#include <catch2/catch_test_macros.hpp>

using namespace NVM::Memory;

TEST_CASE("Constructs", "[SimpleSubArray], [Memory]") {
    REQUIRE_NOTHROW(SimpleSubArray());
}