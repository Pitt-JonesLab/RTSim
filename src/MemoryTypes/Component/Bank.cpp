#include "MemoryTypes/Component/Bank.h"

#include "Logging/Logging.h"
#include "MemoryTypes/Component/BankCommand.h"
#include "Stats/StatBlock.h"

NVM::ComponentType::Bank::Bank() :
    responseConnection(nullptr),
    commandConnection(nullptr),
    currentCommand(BankCommand::Opcode::NO_OP) {}

void NVM::ComponentType::Bank::process() {
    auto busCommand = commandConnection->receive();

    switch (busCommand.getOpcode()) {
        case BankCommand::Opcode::NO_OP:
            break;
        case BankCommand::Opcode::READ:
            currentCommand = busCommand;
            Logging::log() << Logging::LogLevel::EVENT
                           << "Bank received READ command\n";
            break;
        case BankCommand::Opcode::WRITE:
            currentCommand = busCommand;
            Logging::log() << Logging::LogLevel::EVENT
                           << "Bank received WRITE command\n";
            break;
        case BankCommand::Opcode::ACTIVATE:
            break;
        case BankCommand::Opcode::PRECHARGE:
            break;
    }
}

void NVM::ComponentType::Bank::cycle() {}

NVM::Stats::StatBlock NVM::ComponentType::Bank::getStats() {
    return Stats::StatBlock();
}

void NVM::ComponentType::Bank::setResponseConnection(
    Connection<BankResponse>* connection) {
    responseConnection = connection;
}

void NVM::ComponentType::Bank::setCommandConnection(
    Connection<BankCommand>* connection) {
    commandConnection = connection;
}

bool NVM::ComponentType::Bank::busy() const { return false; }
