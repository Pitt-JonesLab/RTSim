#include "Simulation/TraceReader.h"

#include "MockMemorySystem.h"

#include <catch2/catch_test_macros.hpp>
#include <sstream>
#include <string>

using namespace NVM::Simulation;

TEST_CASE("Correct Trace Format", "[TraceReader], [Simulation]") {
    std::string readCommand =
        "0 R 0x1000 "
        "00e8da000000e8d5eb05004883c408c331ed4989d15e4889e24883e4f0505449c7c0a0"
        "0a400048c7c1e00a400048c7c7a4024000e8a7010000f490904883ec08 0";

    std::stringstream ss;
    MockMemorySystem memory;

    SECTION("Reads single command") {
        ss << readCommand;

        TraceReader reader(ss);

        auto traceLine = reader.getNext();
        REQUIRE(traceLine);
        traceLine->issue(&memory);
        REQUIRE(memory.readFlag);
        REQUIRE(memory.lastCycle == 0);
        REQUIRE(memory.lastAddress == 0x1000);
        REQUIRE(memory.lastThread == 0);
        REQUIRE_FALSE(reader.getNext());
    }

    SECTION("Reads multiple commands") {
        for (int i = 0; i < 19; i++) ss << readCommand << '\n';
        ss << readCommand;

        TraceReader reader(ss);

        for (int i = 0; i < 20; i++) {
            auto traceLine = reader.getNext();
            REQUIRE(traceLine);
            traceLine->issue(&memory);
            REQUIRE(memory.readFlag);
            memory.readFlag = false;
        }
        REQUIRE_FALSE(reader.getNext());
    }
}

TEST_CASE("Wrong Format", "[TraceReader], [Simulation]") {
    std::stringstream ss;

    SECTION("Empty line") {
        ss << "\n";
        TraceReader reader(ss);
        REQUIRE_FALSE(reader.getNext());
    }
}