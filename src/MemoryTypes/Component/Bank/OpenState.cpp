#include "MemoryTypes/Component/Bank/OpenState.h"

#include "Logging/Logging.h"
#include "MemoryTypes/Component/Bank/ClosedState.h"
#include "MemoryTypes/Component/Bank/ReadingState.h"
#include "MemoryTypes/Component/Bank/WritingState.h"
#include "MemoryTypes/Simple/Modeling/Decoder.h"
#include "Stats/StatBlock.h"

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
            nextState = std::make_unique<ReadingState>(commandConnection,
                                                       responseConnection, row);
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
            nextState = std::make_unique<WritingState>(commandConnection,
                                                       responseConnection, row);
            break;
        }
        case BankCommand::Opcode::ACTIVATE:
            throw std::runtime_error("Cannot activate open bank!\n");
            break;
        case BankCommand::Opcode::PRECHARGE:
            Logging::log() << Logging::LogLevel::EVENT
                           << "Bank received PRECHARGE command\n";
            nextState = std::make_unique<ClosedState>(commandConnection,
                                                      responseConnection);
            stats.addStat(1, "precharges");
            break;
        default:
            break;
    }
}

void OpenState::cycle() {}

NVM::Stats::ValueStatBlock NVM::ComponentType::OpenState::getStats() {
    return stats;
}

bool NVM::ComponentType::OpenState::busy() const { return false; }
