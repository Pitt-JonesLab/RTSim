#include "Simulation/CycleTimer.h"

#include <catch2/catch_test_macros.hpp>

using NVM::Simulation::CycleTimer;

TEST_CASE("Constructor", "[CycleTimer]") {
    REQUIRE_NOTHROW(CycleTimer());
    REQUIRE_NOTHROW(CycleTimer(100));
}

TEST_CASE("Cycle", "[CycleTimer]") {
    SECTION("No Max") {
        CycleTimer noMax;

        REQUIRE(noMax.cycle(1) == 1);
        REQUIRE(noMax.cycle(10) == 10);
        REQUIRE(noMax.cycle(100) == 100);
    }

    SECTION("Max 10 Cycles") {
        CycleTimer withMax(10);

        REQUIRE(withMax.cycle(1) == 1);
        REQUIRE(withMax.cycle(9) == 9);
        REQUIRE(withMax.cycle(10) == 0);
    }
}