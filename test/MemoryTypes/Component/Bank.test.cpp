#include "MemoryTypes/Component/Bank/Bank.h"

#include "MemoryTypes/Component/Bank/BankCommand.h"
#include "MemoryTypes/Component/Bank/BankResponse.h"

#include <catch2/catch_test_macros.hpp>

using namespace NVM::ComponentType;

TEST_CASE("Works properly", "[Component], [Bank]") {
    Bank bank;
    Connection<BankResponse> responsePort;
    Connection<BankCommand> commandPort;

    bank.setCommandConnection(&commandPort);
    bank.setResponseConnection(&responsePort);

    SECTION("Bank starts CLOSED") {
        REQUIRE(bank.getStateName() == BankInfo::Name::CLOSED);
    }

    SECTION("Activate opens bank") {
        commandPort.issue({BankCommand::Opcode::ACTIVATE});
        commandPort.cycle();
        bank.process();
        bank.cycle();
        REQUIRE(bank.getStateName() == BankInfo::Name::OPEN);
    }

    SECTION("Can't perform transaction on closed bank") {
        commandPort.issue({BankCommand::Opcode::READ});
        commandPort.cycle();
        REQUIRE_THROWS(bank.process());

        commandPort.issue({BankCommand::Opcode::WRITE});
        commandPort.cycle();
        REQUIRE_THROWS(bank.process());

        commandPort.issue({BankCommand::Opcode::TRANSVERSE_READ});
        commandPort.cycle();
        REQUIRE_THROWS(bank.process());

        commandPort.issue({BankCommand::Opcode::COPY});
        commandPort.cycle();
        REQUIRE_THROWS(bank.process());
    }

    SECTION("READING") {
        commandPort.issue({BankCommand::Opcode::ACTIVATE});
        commandPort.cycle();
        bank.process();
        bank.cycle();

        commandPort.issue({BankCommand::Opcode::READ});
        commandPort.cycle();
        bank.process();
        bank.cycle();
        REQUIRE(bank.getStateName() == BankInfo::Name::READING);
    }

    SECTION("WRITING") {
        commandPort.issue({BankCommand::Opcode::ACTIVATE});
        commandPort.cycle();
        bank.process();
        bank.cycle();

        commandPort.issue({BankCommand::Opcode::WRITE});
        commandPort.cycle();
        bank.process();
        bank.cycle();
        REQUIRE(bank.getStateName() == BankInfo::Name::WRITING);
    }
}