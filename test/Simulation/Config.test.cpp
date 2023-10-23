#include "Simulation/Config.h"

#include <catch2/catch_test_macros.hpp>
#include <sstream>

using namespace NVM::Simulation;

TEST_CASE("Gets and sets properly", "[Config], [Simulation]") {
    Config conf;

    conf.set("test1", 5);
    conf.set("test2", std::string("string"));
    conf.set("test3", 1.0);
    conf.set("test4", false);

    SECTION("Gets correct types") {
        REQUIRE(conf.get<int>("test1") == 5);
        REQUIRE(conf.get<std::string>("test2") == "string");
        REQUIRE(conf.get<double>("test3") == 1.0);
        REQUIRE(conf.get<bool>("test4") == false);
    }

    SECTION("Throws on bad get type") {
        REQUIRE_THROWS(conf.get<bool>("test1"));
        REQUIRE_THROWS(conf.get<bool>("test2"));
        REQUIRE_THROWS(conf.get<int>("test3"));
        REQUIRE_THROWS(conf.get<double>("test4"));
    }
}

TEST_CASE("Reads properly", "[Config], [Simulation]") {
    std::stringstream ss;

    SECTION("Ignores comment line") {
        ss.str("; COMMENT\nkey val");
        Config conf = readConfig(ss);

        REQUIRE_THROWS(conf.get<std::string>(";"));
        REQUIRE(conf.get<std::string>("key") == "val");
    }

    SECTION("Reads int") {
        ss.str("key 5");
        Config conf = readConfig(ss);
        REQUIRE(conf.get<int>("key") == 5);
    }

    SECTION("Reads bool") {
        ss.str("key true\nkey2 false");
        Config conf = readConfig(ss);
        REQUIRE(conf.get<bool>("key"));
        REQUIRE_FALSE(conf.get<bool>("key2"));
    }

    SECTION("Reads double") {
        ss.str("key 5.0");
        Config conf = readConfig(ss);
        REQUIRE(conf.get<double>("key") == 5.0);
    }

    SECTION("Ignores comment on key line") {
        ss.str("key val ; COMMENT");
        Config conf = readConfig(ss);
        REQUIRE(conf.get<std::string>("key") == "val");

        ss.str("key val; COMMENT");
        ss.clear();
        conf = readConfig(ss);
        REQUIRE(conf.get<std::string>("key") == "val");
    }

    SECTION("Multiword value") {
        ss.str("key val1 val2");
        Config conf = readConfig(ss);
        REQUIRE(conf.get<std::string>("key") == "val1 val2");
    }

    SECTION("String value int prefix") {
        ss.str("key 50val");
        Config conf = readConfig(ss);
        REQUIRE(conf.get<std::string>("key") == "50val");
    }
}