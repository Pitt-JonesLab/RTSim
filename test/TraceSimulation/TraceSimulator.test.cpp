#include "src/TraceSimulation/TraceSimulator.h"

#include "MockMemorySystem.h"
#include "MockTraceReader.h"
#include "src/TraceSimulation/ReadCommand.h"

#include <catch2/catch_test_macros.hpp>

using namespace NVM::Simulation;

TEST_CASE("Constructs", "[TraceSimulator], [Simulation]") {
    REQUIRE_NOTHROW(
        TraceSimulator(std::unique_ptr<TraceReader>(new MockTraceReader()),
                       std::unique_ptr<MemorySystem>(new MockMemorySystem())));
}

TEST_CASE("Runs simulation", "[TraceSimulator], [Simulation]") {
    SECTION("Reads whole trace file and drains memory") { 
        auto reader = std::make_unique<MockTraceReader>();
        reader->addCommand(std::unique_ptr<TraceCommand>(new ReadCommand(10, 5, DataBlock(), 0)));
        auto readerPtr = reader.get();
        auto memory = std::make_unique<MockMemorySystem>();
        auto memoryPtr = memory.get();

        TraceSimulator simulator(std::move(reader), std::move(memory));
        simulator.run();
        REQUIRE(memoryPtr->isEmpty());
        REQUIRE(readerPtr->commands.empty());
    }

    SECTION("Ends early if timer ends") { 
        auto reader = std::make_unique<MockTraceReader>();
        reader->addCommand(std::unique_ptr<TraceCommand>(new ReadCommand(10, 5, DataBlock(), 0)));
        reader->addCommand(std::unique_ptr<TraceCommand>(new ReadCommand(100, 5, DataBlock(), 0)));
        auto readerPtr = reader.get();
        auto memory = std::make_unique<MockMemorySystem>();
        auto memoryPtr = memory.get();

        TraceSimulator simulator(std::move(reader), std::move(memory), 5);
        simulator.run();
        REQUIRE(!readerPtr->commands.empty()); 
    }
}