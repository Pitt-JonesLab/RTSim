#include "MemoryTypes/Simple/Scheduling/Scheduler/RBScheduler.h"

#include "MemoryTypes/Simple/Modeling/Decoder.h"
#include "Modeling/System/MockSystem.h"

#include <catch2/catch_test_macros.hpp>

using namespace NVM::Scheduling;
using NVM::Modeling::MockSystem;

TEST_CASE("Works properly",
          "[RBScheduler], [Scheduling], [Decoder], [Memory], [Parser]") {
    RBScheduler scheduler;
    MockSystem sys;

    NVM::Modeling::ComponentCounts counts;
    counts.rows = 4;
    counts.channels = 4;
    counts.cols = 4;
    counts.ranks = 4;
    counts.banks = 4;
    // 0x KKBB HHRR CC00 0000 Row bits are R, bank bits are B, channel bits are
    // H, rank bits are K, and column bits are C
    setScheme("RK:BK:CH:R:C", counts);

    SECTION("Row addresses work correctly") {

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
        sys.openRow(0x300); // have to open the row separately
        REQUIRE(scheduler.getRBHits() == 0);

        REQUIRE(scheduler.issue(sys).getAddress() == 0x340);
        sys.openRow(0x340);
        REQUIRE(scheduler.getRBHits() == 1);

        REQUIRE(scheduler.issue(sys).getAddress() == 0x380);
        sys.openRow(0x380);
        REQUIRE(scheduler.getRBHits() == 2);
    }

    SECTION("Basic FIFO order with no row buffer hits") {
        scheduler.enqueue({InstructionType::READ, 0x300});
        scheduler.enqueue({InstructionType::READ, 0x200});
        scheduler.enqueue({InstructionType::READ, 0x100});

        REQUIRE(scheduler.issue(sys).getAddress() == 0x300);
        sys.openRow(0x300);
        REQUIRE(scheduler.getRBHits() == 0);

        REQUIRE(scheduler.issue(sys).getAddress() == 0x200);
        sys.openRow(0x200);
        REQUIRE(scheduler.getRBHits() == 0);

        REQUIRE(scheduler.issue(sys).getAddress() == 0x100);
        sys.openRow(0x100);
        REQUIRE(scheduler.getRBHits() == 0);
    }

    SECTION(
        "Row buffer hits issued first with mix of row buffer hits and misses") {
        scheduler.enqueue({InstructionType::READ, 0x300});
        scheduler.enqueue({InstructionType::READ, 0x200});
        scheduler.enqueue({InstructionType::READ, 0x340});
        scheduler.enqueue({InstructionType::READ, 0x100});
        scheduler.enqueue({InstructionType::READ, 0x380});

        REQUIRE(scheduler.issue(sys).getAddress() == 0x300);
        sys.openRow(0x300);
        REQUIRE(scheduler.getRBHits() == 0);

        REQUIRE(scheduler.issue(sys).getAddress() == 0x340);
        sys.openRow(0x340);
        REQUIRE(scheduler.getRBHits() == 1);

        REQUIRE(scheduler.issue(sys).getAddress() == 0x380);
        sys.openRow(0x380);
        REQUIRE(scheduler.getRBHits() == 2);

        REQUIRE(scheduler.issue(sys).getAddress() == 0x200);
        sys.openRow(0x200);
        REQUIRE(scheduler.getRBHits() == 2);

        REQUIRE(scheduler.issue(sys).getAddress() == 0x100);
        sys.openRow(0x100);
        REQUIRE(scheduler.getRBHits() == 2);
    }
}