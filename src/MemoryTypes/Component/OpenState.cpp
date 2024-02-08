#include "OpenState.h"

#include "Logging/Logging.h"
#include "MemoryTypes/Component/ClosedState.h"

using namespace NVM::ComponentType;

OpenState::OpenState(Connection<BankCommand>* cmd,
                     Connection<BankResponse>* response) :
    BankState(cmd, response) {}

void OpenState::process() {
    auto busCommand = commandConnection->receive();

    switch (busCommand.getOpcode()) {
        case BankCommand::Opcode::READ:
            Logging::log() << Logging::LogLevel::EVENT
                           << "Bank received READ command\n";
            break;
        case BankCommand::Opcode::WRITE:
            Logging::log() << Logging::LogLevel::EVENT
                           << "Bank received WRITE command\n";
            break;
        case BankCommand::Opcode::ACTIVATE:
            throw std::runtime_error("Cannot activate open bank!\n");
            break;
        case BankCommand::Opcode::PRECHARGE:
            Logging::log() << Logging::LogLevel::EVENT
                           << "Bank received PRECHARGE command\n";
            next = std::make_unique<ClosedState>(commandConnection,
                                                 responseConnection);
            break;
        default:
            break;
    }
}

void OpenState::cycle() {}

std::unique_ptr<BankState> OpenState::nextState() { return std::move(next); }
