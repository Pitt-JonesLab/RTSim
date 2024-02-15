#include "MemoryTypes/Component/Bank/OpenState.h"

#include "Logging/Logging.h"
#include "Logging/LogStream.h"
#include "MemoryTypes/Component/Bank/BankCommand.h"
#include "MemoryTypes/Component/Bank/BankResponse.h"
#include "MemoryTypes/Component/Bank/ClosedState.h"
#include "MemoryTypes/Component/Bank/ReadingState.h"
#include "MemoryTypes/Component/Bank/WritingState.h"
#include "MemoryTypes/Simple/Modeling/Decoder.h"
#include "Stats/StatBlock.h"

#include <stdexcept>

using namespace NVM::ComponentType;
using namespace NVM::Modeling;

OpenState::OpenState(BankInfo& i, unsigned int r) :
    State<BankInfo>(i),
    row(r),
    responseCmd(BankResponse::Opcode::NO_OP) {}

void OpenState::process() {
    auto busCommand = info.commandConnection->receive();

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
            nextState = std::make_unique<ReadingState>(info, row);
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
            nextState = std::make_unique<WritingState>(info, row);
            break;
        }
        case BankCommand::Opcode::ACTIVATE:
            throw std::runtime_error("Cannot activate open bank!\n");
            break;
        case BankCommand::Opcode::PRECHARGE:
            Logging::log() << Logging::LogLevel::EVENT
                           << "Bank received PRECHARGE command\n";
            nextState = std::make_unique<ClosedState>(info);
            stats.addStat(1, "precharges");
            break;
        case BankCommand::Opcode::COPY:
            Logging::log() << Logging::LogLevel::EVENT
                           << "Bank received COPY command\n";
            // TODO: make CopyingState
            stats.addStat(1, "row_clones");
            stats.addStat(info.energyCosts.copyEnergy, "copy_energy");
            break;
        case BankCommand::Opcode::TRANSVERSE_READ:
            Logging::log() << Logging::LogLevel::EVENT
                           << "Bank received TRANSVERSE_READ command\n";
            // TODO: make TransverseReadingState
            stats.addStat(1, "transverse_reads");
            stats.addStat(info.energyCosts.transverseReadEnergy,
                          "transverse_read_energy");

            if (info.faultModel.check()) {
                Logging::log() << Logging::LogLevel::EVENT
                               << "TR fault! Bank sending a reiusse response\n";
                stats.addStat(1, "pim_faults");
                responseCmd =
                    BankResponse(BankResponse::Opcode::TR_FAILURE,
                                 busCommand.getAddress(), busCommand.getData());
            }
            stats.addStat(info.faultModel.getUncorrectableFaults(),
                          "uncorrectable_pim_faults");
            info.faultModel.resetUncorrectables();
            break;
        default:
            break;
    }
}

void OpenState::cycle() {
    info.responseConnection->issue(responseCmd);
    responseCmd = BankResponse(BankResponse::Opcode::NO_OP);
}

NVM::Stats::ValueStatBlock NVM::ComponentType::OpenState::getStats() {
    return stats;
}

bool NVM::ComponentType::OpenState::busy() const { return false; }
