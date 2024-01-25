#include "Memory/MemoryFactory.h"

#include "Memory/MemorySystem.h"
#include "Simulation/Config.h"

#include <catch2/catch_test_macros.hpp>

using namespace NVM::Memory;

TEST_CASE("Makes valid simple system", "[MemoryFactory], [Memory]") {
    NVM::Simulation::Config conf;
    conf.set("DBCS", 1);
    conf.set("tACT", 1);
    conf.set("tRP", 1);
    auto system = makeSimpleSystem(conf);
    REQUIRE(system);
}