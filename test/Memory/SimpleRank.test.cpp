#include "Memory/Rank/SimpleRank.h"

#include <catch2/catch_test_macros.hpp>

using namespace NVM::Memory;

TEST_CASE("Constructs", "[SimpleRank], [Memory]") {
    REQUIRE_NOTHROW(SimpleRank());
}