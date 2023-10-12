#include "Memory/MemoryController/SimpleController.h"

#include "Logging/Logging.h"
#include "MockComponent.h"

#include <catch2/catch_test_macros.hpp>
#include <sstream>

using namespace NVM::Memory;
using namespace NVM::Logging;

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

TEST_CASE("Handles requests properly", "[SimpleController], [Memory]") {
    auto interconnectPtr = new MockInterconnect();
    auto interconnect = std::unique_ptr<Interconnect>(interconnectPtr);

    SimpleController controller;
    controller.addInterconnect(std::move(interconnect));

    SECTION("One read at a time") {
        REQUIRE(controller.read(0, {}));
        REQUIRE_FALSE(controller.read(0, {}));
        interconnectPtr->command.notify();
        controller.cycle(1);
        REQUIRE(controller.read(0, {}));
    }

    SECTION("One write at a time") {
        REQUIRE(controller.write(0, {}));
        REQUIRE_FALSE(controller.write(0, {}));
        interconnectPtr->command.notify();
        controller.cycle(1);
        REQUIRE(controller.write(0, {}));
    }
}

TEST_CASE("Logs events", "[SimpleController], [Memory]") {
    auto interconnect = std::unique_ptr<Interconnect>(new MockInterconnect());
    SimpleController controller;
    controller.addInterconnect(std::move(interconnect));
    std::stringstream logString;
    setLogOutput(logString);
    setLogLevel(LogLevel::EVENT);

    SECTION("Logs read") {
        REQUIRE(controller.read(0, {}));
        REQUIRE_FALSE(logString.str().empty());
    }

    SECTION("Logs write") {
        REQUIRE(controller.write(0, {}));
        REQUIRE_FALSE(logString.str().empty());
    }
}