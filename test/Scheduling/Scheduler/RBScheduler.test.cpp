#include "Scheduling/Scheduler/RBScheduler.h"

#include "Modeling/Decoder.h"

#include "Modeling/System/MockSystem.h"

#include <catch2/catch_test_macros.hpp>

using namespace NVM::Scheduling;
using NVM::Modeling::MockSystem;

TEST_CASE("Works properly", "[RBScheduler], [Scheduling], [Decoder], [Memory]") {
    RBScheduler scheduler;
    MockSystem sys;

    NVM::Modeling::ComponentCounts counts;
    counts.rows = 4;
    counts.channels = 4;
    counts.cols = 4;
    counts.ranks = 4;
    counts.banks = 4;

    setScheme("RK:BK:CH:R:C", counts);

    SECTION("Row addresses work correctly"){
        REQUIRE(decodeSymbol(NVM::Modeling::AddressSymbol::ROW, 0x300) == 3);
        REQUIRE(decodeSymbol(NVM::Modeling::AddressSymbol::ROW, 0x340) == 3);
        REQUIRE(decodeSymbol(NVM::Modeling::AddressSymbol::ROW, 0x380) == 3);
        REQUIRE(decodeSymbol(NVM::Modeling::AddressSymbol::ROW, 0x200) == 2);
        REQUIRE(decodeSymbol(NVM::Modeling::AddressSymbol::ROW, 0x100) == 1);
    }

    SECTION("Throws when empty") {
        REQUIRE_THROWS(scheduler.peek(sys));
        REQUIRE_THROWS(scheduler.issue(sys));
    }

    SECTION("Basic FIFO order with all row buffer hits") {
        scheduler.enqueue({InstructionType::READ, 0x300});
        scheduler.enqueue({InstructionType::READ, 0x340});
        scheduler.enqueue({InstructionType::READ, 0x380});

        REQUIRE(scheduler.issue(sys).getAddress() == 0x300);
        REQUIRE(scheduler.issue(sys).getAddress() == 0x340);
        REQUIRE(scheduler.issue(sys).getAddress() == 0x380);
        REQUIRE(scheduler.getRBHits() == 2);
    }

    SECTION("Basic FIFO order with no row buffer hits") {
        scheduler.enqueue({InstructionType::READ, 0x300});
        scheduler.enqueue({InstructionType::READ, 0x200});
        scheduler.enqueue({InstructionType::READ, 0x100});

        REQUIRE(scheduler.issue(sys).getAddress() == 0x300);
        REQUIRE(scheduler.issue(sys).getAddress() == 0x200);
        REQUIRE(scheduler.issue(sys).getAddress() == 0x100);
        REQUIRE(scheduler.getRBHits() == 3);

    }

    MockSystem sys2;

    SECTION("Row buffer hits issued first with mix of row buffer hits and misses") {
        scheduler.enqueue({InstructionType::READ, 0x300});
        scheduler.enqueue({InstructionType::READ, 0x200});
        scheduler.enqueue({InstructionType::READ, 0x340});
        scheduler.enqueue({InstructionType::READ, 0x100});
        scheduler.enqueue({InstructionType::READ, 0x380});

        REQUIRE(scheduler.issue(sys2).getAddress() == 0x300);
        REQUIRE(scheduler.issue(sys2).getAddress() == 0x340);
        REQUIRE(scheduler.issue(sys2).getAddress() == 0x380);
        REQUIRE(scheduler.issue(sys2).getAddress() == 0x200);
        REQUIRE(scheduler.issue(sys2).getAddress() == 0x100);
        REQUIRE(scheduler.getRBHits() == 3);
    
    }
}