#include "src/TraceSimulation/TraceIssuer.h"

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

    SECTION("Issues available request") {
        auto reader = std::make_unique<MockTraceReader>();
        reader->addLine({0, Opcode1::READ});
        TraceIssuer issuer(
            static_cast<std::unique_ptr<TraceReader>>(std::move(reader)), 0);
        REQUIRE(issuer.issue(&memory));
    }

    SECTION("Fails to issue when no request is available") {
        auto reader = std::make_unique<MockTraceReader>();
        TraceIssuer issuer(
            static_cast<std::unique_ptr<TraceReader>>(std::move(reader)), 0);
        REQUIRE_FALSE(issuer.issue(&memory));
    }

    SECTION("Fails to issue when timer is maxed out") {
        auto reader = std::make_unique<MockTraceReader>();
        reader->addLine({10, Opcode1::READ});
        TraceIssuer issuer(
            static_cast<std::unique_ptr<TraceReader>>(std::move(reader)), 1);
        REQUIRE_FALSE(issuer.issue(&memory));
    }
}