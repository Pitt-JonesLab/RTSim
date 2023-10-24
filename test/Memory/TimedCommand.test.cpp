#include "Memory/TimedCommand.h"

#include <catch2/catch_test_macros.hpp>

using namespace NVM::Memory;

TEST_CASE("Works properly", "[TimedCommand], [Memory]") {
    TimedCommand cmd(10);

    SECTION("Finishes after given cycles") {
        cmd.cycle(10);
        REQUIRE(cmd.isDone());
    }

    SECTION("Stays busy before cycles are up") {
        cmd.cycle(5);
        REQUIRE_FALSE(cmd.isDone());
    }
}