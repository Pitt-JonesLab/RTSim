#include "Memory/Bank/SimpleBank.h"

#include <catch2/catch_test_macros.hpp>

using namespace NVM::Memory;

TEST_CASE("Constructs", "[SimpleBank], [Memory]") { REQUIRE_NOTHROW(SimpleBank()); }