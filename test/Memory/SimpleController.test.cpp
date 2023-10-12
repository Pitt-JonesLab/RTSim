#include "Memory/MemoryController/SimpleController.h"

#include "MockComponent.h"

#include <catch2/catch_test_macros.hpp>

using namespace NVM::Memory;

TEST_CASE("Constructs", "[SimpleController], [Memory]") {
    REQUIRE_NOTHROW(SimpleController());
}

TEST_CASE("Sends requests to Interconnect", "[SimpleController], [Memory]") {
    auto interconnectPtr = new MockInterconnect();
    auto interconnect = std::unique_ptr<Interconnect>(interconnectPtr);

    SimpleController controller;
    controller.addInterconnect(std::move(interconnect));

    SECTION("Interconnect gets write") {
        REQUIRE(controller.write(0, {}));
        REQUIRE(interconnectPtr->writeFlag);
    }

    SECTION("Interconnect gets read") {
        REQUIRE(controller.read(0, {}));
        REQUIRE(interconnectPtr->readFlag);
    }
}