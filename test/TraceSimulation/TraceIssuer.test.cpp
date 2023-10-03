#include "src/TraceSimulation/TraceIssuer.h"

#include <catch2/catch_test_macros.hpp>

TEST_CASE("Constructs", "[TraceIssuer], [Simulation], [!mayfail]") {
    REQUIRE(false);
}

TEST_CASE("Issues Correctly", "[TraceIssuer], [Simulation], [!mayfail]") {
    SECTION("Issues available request") { REQUIRE(false); }

    SECTION("Fails to issue when no request is available") { REQUIRE(false); }

    SECTION("Fails to issue when timer is maxed out") { REQUIRE(false); }
}