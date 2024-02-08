#include "MemoryTypes/Component/MemoryController.h"

#include "Logging/Logging.h"
#include "MemoryTypes/Component/BankCommand.h"
#include "MemoryTypes/Component/MemoryCommand.h"
#include "Stats/StatBlock.h"

NVM::ComponentType::MemoryController::MemoryController() :
    commandConnection(nullptr),
    working(false),
    currentCommand(MemoryCommand::Opcode::NO_OP),
    nextCommand(BankCommand::Opcode::NO_OP) {}

void NVM::ComponentType::MemoryController::process() {}

void NVM::ComponentType::MemoryController::cycle() {
    if (working && commandConnection->validateTiming(nextCommand)) {
        switch (nextCommand.getOpcode()) {
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
        commandConnection->issue(nextCommand);
        working = false;
    }
}

NVM::Stats::StatBlock NVM::ComponentType::MemoryController::getStats() {
    return Stats::StatBlock();
}

bool NVM::ComponentType::MemoryController::issue(MemoryCommand command) {
    if (working) return false;
    working = true;
    currentCommand = command;
    switch (currentCommand.getOpcode()) {
        case MemoryCommand::Opcode::NO_OP:
            nextCommand = {BankCommand::Opcode::NO_OP,
                           currentCommand.getAddress(),
                           currentCommand.getData()};
            break;
        case MemoryCommand::Opcode::READ:
            nextCommand = {BankCommand::Opcode::READ,
                           currentCommand.getAddress(),
                           currentCommand.getData()};
            break;
        case MemoryCommand::Opcode::WRITE:
            nextCommand = {BankCommand::Opcode::READ,
                           currentCommand.getAddress(),
                           currentCommand.getData()};
            break;
    }
    return true;
}

void NVM::ComponentType::MemoryController::setCommandConnection(
    Connection<BankCommand>* connection) {
    commandConnection = connection;
}

bool NVM::ComponentType::MemoryController::busy() const { return working; }
