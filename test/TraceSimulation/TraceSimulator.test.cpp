#include "src/TraceSimulation/TraceSimulator.h"

#include "MockMemorySystem.h"
#include "MockTraceReader.h"

#include <catch2/catch_test_macros.hpp>

using namespace NVM::Simulation;

TEST_CASE("Constructs", "[TraceSimulator], [Simulation]") {
    REQUIRE_NOTHROW(
        TraceSimulator(std::unique_ptr<TraceReader>(new MockTraceReader()),
                       std::unique_ptr<MemorySystem>(new MockMemorySystem())));
}

TEST_CASE("Runs simulation", "[TraceSimulator], [Simulation], [!mayfail]") {
    SECTION("Reads whole trace file and drains memory") { REQUIRE(false); }
    SECTION("Ends early if timer ends") { REQUIRE(false); }
}