#include "src/TraceSimulation/TraceRequestMaker.h"

#include <catch2/catch_test_macros.hpp>

TEST_CASE("Constructs", "[TraceRequestMaker], [Simulation], [!mayfail]") {
    REQUIRE(false);
}

TEST_CASE("Converts TraceLines to Requests",
          "[TraceRequestMaker], [Simulation], [!mayfail]") {
    SECTION("Read op") { REQUIRE(false); }
    SECTION("Write op") { REQUIRE(false); }
    SECTION("PIM op") { REQUIRE(false); }
    SECTION("Transverse Write op") { REQUIRE(false); }
    SECTION("Shift op") { REQUIRE(false); }
}