#include "Memory/MemorySystem/SimpleSystem.h"

#include <catch2/catch_test_macros.hpp>

using namespace NVM::Memory;

TEST_CASE("Constructs", "[SimpleSystem], [Memory]") {
    REQUIRE_NOTHROW(SimpleSystem());
}