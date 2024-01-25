#include "MemoryTypes/Simple/Scheduling/Scheduler/FIFOScheduler.h"

#include "MemoryTypes/Simple/Modeling/System/MockSystem.h"

#include <catch2/catch_test_macros.hpp>

using namespace NVM::Scheduling;
using NVM::Modeling::MockSystem;

TEST_CASE("Works properly", "[FIFOScheduler], [Scheduling]") {
    FIFOScheduler scheduler;
    MockSystem sys;

    SECTION("Throws when empty") {
        REQUIRE_THROWS(scheduler.peek(sys));
        REQUIRE_THROWS(scheduler.issue(sys));
    }

    SECTION("Basic FIFO order") {
        scheduler.enqueue({InstructionType::READ, 0});
        scheduler.enqueue({InstructionType::READ, 1});
        scheduler.enqueue({InstructionType::READ, 2});

        REQUIRE(scheduler.issue(sys).getAddress() == 0);
        REQUIRE(scheduler.issue(sys).getAddress() == 1);
        REQUIRE(scheduler.issue(sys).getAddress() == 2);
    }
}