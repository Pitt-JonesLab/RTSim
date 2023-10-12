#include "Memory/MemorySystem/SimpleSystem.h"

#include "Logging/Logging.h"
#include "MockComponent.h"

#include <catch2/catch_test_macros.hpp>
#include <sstream>

using namespace NVM::Memory;
using namespace NVM::Logging;

TEST_CASE("Constructs", "[SimpleSystem], [Memory]") {
    REQUIRE_NOTHROW(SimpleSystem());
}

TEST_CASE("Sends requests to Controller", "[SimpleSystem], [Memory]") {
    auto controllerPtr = new MockController();
    auto controller = std::unique_ptr<MemoryController>(controllerPtr);

    SimpleSystem system;
    system.addController(std::move(controller));

    SECTION("Controller gets write") {
        REQUIRE(system.write(0, {}, 0, 0));
        REQUIRE(controllerPtr->writeFlag);
    }

    SECTION("Controller gets read") {
        REQUIRE(system.read(0, {}, 0, 0));
        REQUIRE(controllerPtr->readFlag);
    }
}

TEST_CASE("Handles requests properly", "[SimpleSystem], [Memory]") {
    auto controllerPtr = new MockController();
    auto controller = std::unique_ptr<MemoryController>(controllerPtr);

    SimpleSystem system;
    system.addController(std::move(controller));

    SECTION("One read at a time") {
        REQUIRE(system.read(0, {}, 0, 0));
        REQUIRE_FALSE(system.read(0, {}, 0, 0));
        controllerPtr->command.notify();
        system.cycle(1);
        REQUIRE(system.read(0, {}, 0, 0));
    }

    SECTION("One write at a time") {
        REQUIRE(system.write(0, {}, 0, 0));
        REQUIRE_FALSE(system.write(0, {}, 0, 0));
        controllerPtr->command.notify();
        system.cycle(1);
        REQUIRE(system.write(0, {}, 0, 0));
    }
}

TEST_CASE("Logs events", "[SimpleSystem], [Memory]") {
    auto controller = std::unique_ptr<MemoryController>(new MockController());
    SimpleSystem system;
    system.addController(std::move(controller));
    std::stringstream logString;
    setLogOutput(logString);
    setLogLevel(LogLevel::EVENT);

    SECTION("Logs read") {
        REQUIRE(system.read(0, {}, 0, 0));
        REQUIRE_FALSE(logString.str().empty());
    }

    SECTION("Logs write") {
        REQUIRE(system.write(0, {}, 0, 0));
        REQUIRE_FALSE(logString.str().empty());
    }
}