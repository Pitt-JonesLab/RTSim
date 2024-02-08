#include "MemoryTypes/Component/MemoryController.h"

#include "Logging/Logging.h"
#include "MemoryTypes/Component/BankCommand.h"
#include "MemoryTypes/Component/MemoryCommand.h"
#include "Stats/StatBlock.h"

NVM::ComponentType::MemoryController::MemoryController() :
    commandConnection(nullptr),
    issued(false),
    nextCommand(MemoryCommand::Opcode::NO_OP),
    bankModel(),
    reads(0),
    writes(0),
    activates(0),
    precharges(0) {}

void NVM::ComponentType::MemoryController::process() {}

void NVM::ComponentType::MemoryController::cycle() {
    if (bankQueue.size() &&
        commandConnection->validateTiming(bankQueue.front())) {
        switch (bankQueue.front().getOpcode()) {
            case BankCommand::Opcode::ACTIVATE:
                Logging::log() << Logging::LogLevel::EVENT
                               << "MemoryController issuing ACTIVATE command\n";
                activates++;
                break;
            case BankCommand::Opcode::PRECHARGE:
                Logging::log()
                    << Logging::LogLevel::EVENT
                    << "MemoryController issuing PRECHARGE command\n";
                precharges++;
                break;
            case BankCommand::Opcode::READ:
                Logging::log() << Logging::LogLevel::EVENT
                               << "MemoryController issuing READ command\n";
                reads++;
                break;
            case BankCommand::Opcode::WRITE:
                Logging::log() << Logging::LogLevel::EVENT
                               << "MemoryController issuing WRITE command\n";
                writes++;
                break;
            default:
                break;
        }
        commandConnection->issue(bankQueue.front());
        bankQueue.pop();
    }

    if (issued) {
        auto bankCommands = parser.parse(nextCommand, bankModel);
        for (auto& cmd : bankCommands) bankQueue.push(cmd);
    }
    issued = false;
}

NVM::Stats::ValueStatBlock
NVM::ComponentType::MemoryController::getStats(std::string tag) {
    Stats::ValueStatBlock stats(tag);

    stats.addStat(reads, "reads");
    stats.addStat(writes, "writes");
    stats.addStat(activates, "activates");
    stats.addStat(precharges, "precharges");

    return stats;
}

bool NVM::ComponentType::MemoryController::issue(MemoryCommand command) {
    if (bankQueue.size() >= 10) return false;
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
