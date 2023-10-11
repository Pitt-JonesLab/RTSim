#include "Memory/Interconnect/SimpleInterconnect.h"

#include <catch2/catch_test_macros.hpp>

using namespace NVM::Memory;

TEST_CASE("Constructs", "[SimpleInterconnect], [Memory]") { REQUIRE_NOTHROW(SimpleInterconnect()); }