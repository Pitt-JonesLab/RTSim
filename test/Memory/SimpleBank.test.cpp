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
        REQUIRE(bank.write(0, {}, 0, 0));
        REQUIRE(subArrayPtr->writeFlag);
    }

    SECTION("SubArray gets read") {
        REQUIRE(bank.read(0, {}, 0, 0));
        REQUIRE(subArrayPtr->readFlag);
    }
}