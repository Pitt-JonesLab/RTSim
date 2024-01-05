#include "Modeling/Bank/SimpleBank.h"

#include "Modeling/Decoder.h"
#include "Modeling/SubArray/MockSubArray.h"

#include <catch2/catch_test_macros.hpp>

using namespace NVM::Modeling;

TEST_CASE("Works properly", "[SimpleBank], [Modeling]") {
    SimpleBank bank;
    Address address = 0;

    MockSubArray* subArray = new MockSubArray();
    bank.addSubArray(std::unique_ptr<SubArray>(subArray));

    SECTION("isOpen") {
        REQUIRE_FALSE(bank.isOpen(address));
        bank.openRow(address);
        REQUIRE(bank.isOpen(address));
        bank.closeRow(address);
        REQUIRE_FALSE(bank.isOpen(address));
    }

    SECTION("openRow") {
        REQUIRE_THROWS(bank.getOpenRow(address));
        bank.openRow(address);
        REQUIRE(bank.getOpenRow(address) == 0);
        REQUIRE_THROWS(bank.openRow(address));
        bank.closeRow(address);
        REQUIRE_THROWS(bank.getOpenRow(address));
        address = replaceSymbol(address, AddressSymbol::ROW, 1);
        bank.openRow(address);
        REQUIRE(bank.getOpenRow(address) == 1);
    }
}