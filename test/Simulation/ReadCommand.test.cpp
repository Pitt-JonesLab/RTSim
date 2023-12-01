#include "Simulation/Command/ReadCommand.h"

#include "MockMemorySystem.h"

#include <catch2/catch_test_macros.hpp>

using namespace NVM::Simulation;

TEST_CASE("Issues correctly", "[ReadCommand], [Simulation]") {
    ReadCommand command(5, 100, DataBlock({5}), 1);
    MockMemorySystem memory;

    SECTION("Succeeds if issue cycle reached and memory available") {
        memory.available = true;
        memory.currentCycle = 10;
        REQUIRE(command.issue(&memory));
        REQUIRE(memory.lastCycle == 5);
        REQUIRE(memory.lastAddress == 100);
        REQUIRE(memory.lastData.bytes == std::array<uint8_t, 64>({5}));
        REQUIRE(memory.lastThread == 1);
    }

    SECTION("Fails if memory not available") {
        memory.available = false;
        REQUIRE_FALSE(command.issue(&memory));
    }

    SECTION("Fails if issue cycle not reached") {
        memory.available = true;
        memory.currentCycle = 0;
        REQUIRE_FALSE(command.issue(&memory));
    }
}