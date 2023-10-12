#include "Memory/Bank/SimpleBank.h"

#include "MockComponent.h"

#include <catch2/catch_test_macros.hpp>

using namespace NVM::Memory;

TEST_CASE("Constructs", "[SimpleBank], [Memory]") {
    REQUIRE_NOTHROW(SimpleBank());
}

TEST_CASE("Sends requests to SubArray", "[SimpleBank], [Memory]") {
    auto subArrayPtr = new MockSubArray();
    auto subArray = std::unique_ptr<SubArray>(subArrayPtr);

    SimpleBank bank;
    bank.addSubArray(std::move(subArray));

    SECTION("SubArray gets write") {
        REQUIRE(bank.write(0, {}));
        REQUIRE(subArrayPtr->writeFlag);
    }

    SECTION("SubArray gets read") {
        REQUIRE(bank.read(0, {}));
        REQUIRE(subArrayPtr->readFlag);
    }
}

TEST_CASE("Handles requests properly", "[SimpleBank], [Memory]") {
    auto subArrayPtr = new MockSubArray();
    auto subArray = std::unique_ptr<SubArray>(subArrayPtr);

    SimpleBank bank;
    bank.addSubArray(std::move(subArray));

    SECTION("One read at a time") {
        REQUIRE(bank.read(0, {}));
        REQUIRE_FALSE(bank.read(0, {}));
        subArrayPtr->command.notify();
        bank.cycle(1);
        REQUIRE(bank.read(0, {}));
    }

    SECTION("One write at a time") {
        REQUIRE(bank.write(0, {}));
        REQUIRE_FALSE(bank.write(0, {}));
        subArrayPtr->command.notify();
        bank.cycle(1);
        REQUIRE(bank.write(0, {}));
    }
}