#include "OpenState.h"

#include "Logging/Logging.h"
#include "MemoryTypes/Component/ClosedState.h"
#include "MemoryTypes/Simple/Modeling/Decoder.h"

#include <stdexcept>

using namespace NVM::ComponentType;
using namespace NVM::Modeling;

OpenState::OpenState(Connection<BankCommand>* cmd,
                     Connection<BankResponse>* response, unsigned int r) :
    BankState(cmd, response),
    row(r) {}

void OpenState::process() {
    auto busCommand = commandConnection->receive();

    switch (busCommand.getOpcode()) {
        case BankCommand::Opcode::READ: {
            auto cmdRow = decodeSymbol(AddressSymbol::ROW,
                                       busCommand.getAddress().getData());
            if (row != cmdRow)
                throw std::runtime_error(
                    "READ command is for a different row!\n");
            Logging::log() << Logging::LogLevel::EVENT
                           << "Bank received READ command for row " << row
                           << " address " << busCommand.getAddress().getData()
                           << "\n";
            break;
        }
        case BankCommand::Opcode::WRITE: {
            auto cmdRow = decodeSymbol(AddressSymbol::ROW,
                                       busCommand.getAddress().getData());
            if (row != cmdRow)
                throw std::runtime_error(
                    "WRITE command is for a different row!\n");
            Logging::log() << Logging::LogLevel::EVENT
                           << "Bank received WRITE command for row " << row
                           << "\n";
            break;
        }
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
