#include "Simulation/TraceSimulator.h"

#include "MockMemorySystem.h"
#include "MockTraceReader.h"

#include <catch2/catch_test_macros.hpp>

using namespace NVM::Simulation;

TEST_CASE("Constructs", "[TraceSimulator], [Simulation]") {
    REQUIRE_NOTHROW(
        TraceSimulator(std::unique_ptr<TraceReader>(new MockTraceReader()),
                       std::unique_ptr<Commandable>(new MockMemorySystem())));
}

TEST_CASE("Runs simulation", "[TraceSimulator], [Simulation]") {
    SECTION("Reads whole trace file and drains memory") {
        auto reader = std::make_unique<MockTraceReader>();
        reader->addCommand([](Commandable& receiver) {
            return receiver.read(Address(), RowData());
        });
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
        for (int i = 0; i < 20; i++) {
            reader->addCommand([](Commandable& receiver) {
                return receiver.read(Address(5), RowData());
            });
        }
        auto readerPtr = reader.get();
        auto memory = std::make_unique<MockMemorySystem>();
        auto memoryPtr = memory.get();

        TraceSimulator simulator(std::move(reader), std::move(memory), 1);
        memoryPtr->working = false;
        memoryPtr->available = false;
        simulator.run();
        REQUIRE(!readerPtr->commands.empty());
    }
}