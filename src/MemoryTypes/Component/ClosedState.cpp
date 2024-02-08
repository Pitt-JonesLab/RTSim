#include "MemoryTypes/Component/ClosedState.h"

#include "Logging/Logging.h"
#include "MemoryTypes/Component/BankCommand.h"

NVM::ComponentType::ClosedState::ClosedState(
    Connection<BankCommand>* cmd, Connection<BankResponse>* response) :
    BankState(cmd, response) {}

void NVM::ComponentType::ClosedState::process() {
    auto busCommand = commandConnection->receive();

    switch (busCommand.getOpcode()) {
        case BankCommand::Opcode::NO_OP:
            break;
        case BankCommand::Opcode::READ:
            Logging::log() << Logging::LogLevel::EVENT
                           << "Bank received READ command\n";
            break;
        case BankCommand::Opcode::WRITE:
            Logging::log() << Logging::LogLevel::EVENT
                           << "Bank received WRITE command\n";
            break;
        case BankCommand::Opcode::ACTIVATE:
            break;
        case BankCommand::Opcode::PRECHARGE:
            break;
    }
}

void NVM::ComponentType::ClosedState::cycle() {}

std::unique_ptr<NVM::ComponentType::BankState>
NVM::ComponentType::ClosedState::nextState() {
    return nullptr;
}
