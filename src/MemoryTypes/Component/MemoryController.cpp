#include "MemoryTypes/Component/MemoryController.h"

#include "Logging/Logging.h"
#include "MemoryTypes/Component/BankCommand.h"
#include "MemoryTypes/Component/MemoryCommand.h"
#include "Stats/StatBlock.h"

NVM::ComponentType::MemoryController::MemoryController() :
    commandConnection(nullptr),
    issued(false),
    nextCommand(BankCommand::Opcode::NO_OP) {
    bankQueue.push({BankCommand::Opcode::ACTIVATE});
}

void NVM::ComponentType::MemoryController::process() {}

void NVM::ComponentType::MemoryController::cycle() {
    if (bankQueue.size() &&
        commandConnection->validateTiming(bankQueue.front())) {
        switch (bankQueue.front().getOpcode()) {
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
    if (issued) bankQueue.push(nextCommand);
    issued = false;
}

NVM::Stats::StatBlock NVM::ComponentType::MemoryController::getStats() {
    return Stats::StatBlock();
}

bool NVM::ComponentType::MemoryController::issue(MemoryCommand command) {
    if (bankQueue.size() == 10) return false;
    switch (command.getOpcode()) {
        case MemoryCommand::Opcode::NO_OP:
            nextCommand = {BankCommand::Opcode::NO_OP, command.getAddress(),
                           command.getData()};
            break;
        case MemoryCommand::Opcode::READ:
            nextCommand = {BankCommand::Opcode::READ, command.getAddress(),
                           command.getData()};
            break;
        case MemoryCommand::Opcode::WRITE:
            nextCommand = {BankCommand::Opcode::READ, command.getAddress(),
                           command.getData()};
            break;
    }
    issued = true;

    return true;
}

void NVM::ComponentType::MemoryController::setCommandConnection(
    Connection<BankCommand>* connection) {
    commandConnection = connection;
}

bool NVM::ComponentType::MemoryController::busy() const {
    return bankQueue.size() || issued;
}
