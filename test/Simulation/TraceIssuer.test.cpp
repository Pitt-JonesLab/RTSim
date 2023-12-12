#include "Simulation/TraceIssuer.h"

#include "MockMemorySystem.h"
#include "MockTraceReader.h"

#include <catch2/catch_test_macros.hpp>

using NVM::Simulation::MockTraceReader;
using NVM::Simulation::TraceIssuer;
using NVM::Simulation::TraceReader;

using namespace NVM::Simulation;

TEST_CASE("Constructs", "[TraceIssuer], [Simulation]") {
    auto reader = std::make_unique<MockTraceReader>();
    REQUIRE_NOTHROW(TraceIssuer(
        static_cast<std::unique_ptr<TraceReader>>(std::move(reader)), 0));
}

TEST_CASE("Issues Correctly", "[TraceIssuer], [Simulation]") {
    MockMemorySystem memory;

    auto reader = std::make_unique<MockTraceReader>();

    SECTION("Issues available request") {
        reader->addCommand([](Commandable& receiver) {
            return receiver.read(Address(0), RowData());
        });
        TraceIssuer issuer(
            static_cast<std::unique_ptr<TraceReader>>(std::move(reader)), 0);
        REQUIRE(issuer.issueCommand(memory));
    }

    SECTION("Issues several requests - available memory") {
        for (int i = 0; i < 10; i++) {
            reader->addCommand([](Commandable& receiver) {
                return receiver.read(Address(0), RowData());
            });
        }
        TraceIssuer issuer(
            static_cast<std::unique_ptr<TraceReader>>(std::move(reader)), 0);
        for (int i = 0; i < 10; i++) {
            REQUIRE(issuer.issueCommand(memory));
        }
    }

    SECTION("Issues several requests - must cycle") {
        for (int i = 0; i < 10; i++) {
            reader->addCommand([](Commandable& receiver) {
                return receiver.read(Address(0), RowData());
            });
        }
        TraceIssuer issuer(
            static_cast<std::unique_ptr<TraceReader>>(std::move(reader)), 0);
        for (int i = 0; i < 10; i++) {
            REQUIRE(issuer.issueCommand(memory));
            memory.available = false;
        }
    }

    SECTION("Fails to issue when no request is available") {
        TraceIssuer issuer(
            static_cast<std::unique_ptr<TraceReader>>(std::move(reader)), 0);
        REQUIRE_FALSE(issuer.issueCommand(memory));
    }

    SECTION("Fails to issue when timer is maxed out") {
        reader->addCommand([](Commandable& receiver) {
            return receiver.read(Address(0), RowData());
        });
        reader->addCommand([](Commandable& receiver) {
            return receiver.read(Address(0), RowData());
        });
        memory.available = false;
        TraceIssuer issuer(
            static_cast<std::unique_ptr<TraceReader>>(std::move(reader)), 1);
        REQUIRE(issuer.issueCommand(memory));
        memory.available = false;
        REQUIRE_FALSE(issuer.issueCommand(memory));
    }
}