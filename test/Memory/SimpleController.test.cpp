#include "Memory/MemoryController/SimpleController.h"

#include <catch2/catch_test_macros.hpp>

using namespace NVM::Memory;

TEST_CASE("Constructs", "[SimpleController], [Memory]") { REQUIRE_NOTHROW(SimpleController()); }