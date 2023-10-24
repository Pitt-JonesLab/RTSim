#include "Memory/WaitingCommand.h"

#include "MockComponent.h"

#include <catch2/catch_test_macros.hpp>

using namespace NVM::Memory;

TEST_CASE("Works properly", "[WaitingCommand], [Memory]") {
    WaitingCommand cmd;
    MockCommand mock;
    mock.setParent(&cmd);

    SECTION("Does not finish until child") {
        REQUIRE_FALSE(cmd.isDone());
        mock.notify();
        REQUIRE(cmd.isDone());
    }
}