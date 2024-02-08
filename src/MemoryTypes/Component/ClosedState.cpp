#include "MemoryTypes/Component/ClosedState.h"

#include "Logging/Logging.h"
#include "MemoryTypes/Component/BankCommand.h"
#include "MemoryTypes/Component/OpenState.h"
#include "MemoryTypes/Simple/Modeling/Decoder.h"

using namespace NVM::Modeling;

NVM::ComponentType::ClosedState::ClosedState(
    Connection<BankCommand>* cmd, Connection<BankResponse>* response) :
    BankState(cmd, response) {}

void NVM::ComponentType::ClosedState::process() {
    auto busCommand = commandConnection->receive();

    switch (busCommand.getOpcode()) {
        case BankCommand::Opcode::READ:
            throw std::runtime_error("Cannot read from closed bank!\n");
            break;
        case BankCommand::Opcode::WRITE:
            throw std::runtime_error("Cannot write to closed bank!\n");

            break;
        case BankCommand::Opcode::PRECHARGE:
            throw std::runtime_error("Cannot precharge closed bank!\n");
            break;
        case BankCommand::Opcode::ACTIVATE: {
            auto row = decodeSymbol(AddressSymbol::ROW,
                                    busCommand.getAddress().getData());
            Logging::log() << Logging::LogLevel::EVENT
                           << "Bank received ACTIVATE command for row " << row
                           << "\n";
            next = std::make_unique<OpenState>(commandConnection,
                                               responseConnection, row);
            break;
        }
        default:
            break;
    }
}

void NVM::ComponentType::ClosedState::cycle() {}

std::unique_ptr<NVM::ComponentType::BankState>
NVM::ComponentType::ClosedState::nextState() {
    return std::move(next);
}
