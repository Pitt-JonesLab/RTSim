#include "Memory/ChainedCommand.h"

#include "MockComponent.h"

#include <catch2/catch_test_macros.hpp>

using namespace NVM::Memory;

MockCommand* mock;

Command* makeMock() {
    return mock;
}

TEST_CASE("Works properly", "[ChainedCommand], [Memory]") {
    mock = new MockCommand();
    ChainedCommand cmd({&makeMock, &makeMock});

    SECTION("Waits on all Commands in chain") {
        REQUIRE_FALSE(cmd.isDone());
        
        // Make new mock and finish current
        auto currentMock = mock;
        mock = new MockCommand();
        currentMock->notify();
        REQUIRE_FALSE(cmd.isDone());
        
        // cmd is waiting on mock now
        mock->notify();
        REQUIRE(cmd.isDone());
    }

    SECTION("Not busy when waiting for next Command") {
        auto currentMock = mock;
        REQUIRE(cmd.isBusy());
        mock = nullptr;
        currentMock->notify();

        REQUIRE_FALSE(cmd.isBusy());
        mock = new MockCommand();
        cmd.getNextStep();
        REQUIRE(cmd.isBusy());
    }
}