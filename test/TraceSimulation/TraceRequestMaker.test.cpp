#include "src/TraceSimulation/TraceRequestMaker.h"

#include "include/NVMainRequest.h"
#include "MockTraceReader.h"

#include <catch2/catch_test_macros.hpp>

using namespace NVM::Simulation;
using namespace NVM;

TEST_CASE("Constructs", "[TraceRequestMaker], [Simulation]") {
    REQUIRE_NOTHROW(
        TraceRequestMaker(std::unique_ptr<TraceReader>(new MockTraceReader)));
}

TEST_CASE("Converts TraceLines to Requests",
          "[TraceRequestMaker], [Simulation]") {
    // Only test READ and WRITE for now
    SECTION("Read op") {
        auto reader = std::make_unique<MockTraceReader>();
        reader->addLine({5, Opcode1::READ});
        TraceRequestMaker reqMaker(
            static_cast<std::unique_ptr<TraceReader>>(std::move(reader)));

        // First request should have type READ
        auto req = reqMaker.getRequest();
        REQUIRE(req);
        REQUIRE(req->type == READ);
        REQUIRE(req->arrivalCycle == 5);

        // Only one request
        req = reqMaker.getRequest();
        REQUIRE_FALSE(req);
    }
    SECTION("Write op") {
        auto reader = std::make_unique<MockTraceReader>();
        reader->addLine({5, Opcode1::WRITE});
        TraceRequestMaker reqMaker(
            static_cast<std::unique_ptr<TraceReader>>(std::move(reader)));

        // First request should have type WRITE
        auto req = reqMaker.getRequest();
        REQUIRE(req);
        REQUIRE(req->type == WRITE);
        REQUIRE(req->arrivalCycle == 5);

        // Only one request
        req = reqMaker.getRequest();
        REQUIRE_FALSE(req);
    }
    SECTION("PIM op") {}
    SECTION("Transverse Write op") {}
    SECTION("Shift op") {}
}