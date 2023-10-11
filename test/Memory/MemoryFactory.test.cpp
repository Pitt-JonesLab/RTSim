#include "Memory/MemoryFactory.h"

#include "Memory/MemorySystem.h"

#include <catch2/catch_test_macros.hpp>

using namespace NVM::Memory;

TEST_CASE("Makes valid simple system", "[MemoryFactory], [Memory]") {
    auto system = makeSimpleSystem();
    REQUIRE(system);
}