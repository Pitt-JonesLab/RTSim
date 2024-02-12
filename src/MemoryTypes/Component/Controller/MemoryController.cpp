#include "MemoryTypes/Component/Controller/MemoryController.h"

#include "Logging/Logging.h"
#include "MemoryTypes/Component/Bank/BankCommand.h"
#include "MemoryTypes/Component/Controller/Scheduler.h"
#include "MemoryTypes/Component/System/MemoryCommand.h"
#include "Stats/StatBlock.h"

NVM::ComponentType::MemoryController::MemoryController() :
    commandConnection(nullptr),
    issued(false),
    nextCommand(MemoryCommand::Opcode::NO_OP),
    bankModel() {}

void NVM::ComponentType::MemoryController::process() {
    if (bankQueue.empty() && scheduler.getSize()) {
        auto bankCommands =
            parser.parse(scheduler.getNext(bankModel), bankModel);
        for (auto& cmd : bankCommands) bankQueue.push(cmd);
    }

    if (issued) {
        scheduler.issue(nextCommand);
    }
    issued = false;
}

void NVM::ComponentType::MemoryController::cycle() {
    if (bankQueue.size() &&
        commandConnection->validateTiming(bankQueue.front())) {
        switch (bankQueue.front().getOpcode()) {
            case BankCommand::Opcode::ACTIVATE:
                Logging::log() << Logging::LogLevel::EVENT
                               << "MemoryController issuing ACTIVATE command\n";
                break;
            case BankCommand::Opcode::PRECHARGE:
                Logging::log()
                    << Logging::LogLevel::EVENT
                    << "MemoryController issuing PRECHARGE command\n";
                break;
            case BankCommand::Opcode::READ:
                Logging::log() << Logging::LogLevel::EVENT
                               << "MemoryController issuing READ command\n";
                break;
            case BankCommand::Opcode::WRITE:
                Logging::log() << Logging::LogLevel::EVENT
                               << "MemoryController issuing WRITE command\n";
                break;
            default:
                break;
        }
        commandConnection->issue(bankQueue.front());
        bankQueue.pop();
    }
}

NVM::Stats::ValueStatBlock
NVM::ComponentType::MemoryController::getStats(std::string tag) {
    Stats::ValueStatBlock stats(tag);

    stats.addStat(scheduler.getRowBufferHits(), "row_buffer_hits");

    return stats;
}

bool NVM::ComponentType::MemoryController::issue(MemoryCommand command) {
    if (scheduler.getSize() >= 20) return false;
    nextCommand = command;
    return issued = true;
}

void NVM::ComponentType::MemoryController::setCommandConnection(
    Connection<BankCommand>* connection) {
    commandConnection = connection;
}

bool NVM::ComponentType::MemoryController::busy() const {
    return bankQueue.size() || issued;
}
