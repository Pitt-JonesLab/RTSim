#include "Logging/LogStream.h"

#include <catch2/catch_test_macros.hpp>
#include <sstream>

using namespace NVM;
using namespace NVM::Logging;

TEST_CASE("Constructs with sstream", "[LogStream], [Logging]") {
    std::stringstream ss;

    REQUIRE_NOTHROW(LogStream(ss));
}

TEST_CASE("Logs based on LogLevel", "[LogStream], [Logging]") {
    std::stringstream ss;

    LogStream log(ss);
    log.setGlobalLevel(LogLevel::STAT);

    SECTION("Lower verbosity gets printed") {
        log << LogLevel::STAT << "test";
        REQUIRE(ss.str() == "test");
        ss.str(std::string());
    }

    SECTION("Higher verbosity gets ignored") {
        log << LogLevel::DEBUG << "test";
        REQUIRE(ss.str().empty());
    }
}

TEST_CASE("Stat tags work properly", "[LogStream], [Logging]") {
    std::stringstream ss;

    LogStream log(ss);
    log.setGlobalLevel(LogLevel::STAT);

    SECTION("No tags") {
        log << Logging::useTags;
        REQUIRE(ss.str().empty());
    }

    SECTION("One tag") {
        log.addStatTag("test");
        log << Logging::useTags;
        REQUIRE(ss.str() == "test");
    }

    SECTION("A few tags") {
        log.addStatTag("foo");
        log.addStatTag("bar");
        log.addStatTag("baz");
        log << Logging::useTags;
        REQUIRE(ss.str() == "foo.bar.baz");
    }

    SECTION("Can pop tags") {
        log.addStatTag("foo");
        log.addStatTag("bar");
        log.addStatTag("baz");
        log.popStatTag();
        log << Logging::useTags;
        REQUIRE(ss.str() == "foo.bar");
        ss.str(std::string());
        log.popStatTag();
        log << Logging::useTags;
        REQUIRE(ss.str() == "foo");
    }
}