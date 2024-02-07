#include "MemoryTypes/Component/MemoryController.h"

#include "Logging/Logging.h"
#include "MemoryTypes/Component/MemoryCommand.h"
#include "Stats/StatBlock.h"

NVM::ComponentType::MemoryController::MemoryController() :
    commandConnection(nullptr),
    working(false),
    currentCommand(MemoryCommand::Opcode::NO_OP) {}

void NVM::ComponentType::MemoryController::process() {}

void NVM::ComponentType::MemoryController::cycle() {
    if (working) {
        switch (currentCommand.getOpcode()) {
            case MemoryCommand::Opcode::READ:
                Logging::log() << Logging::LogLevel::EVENT
                               << "MemoryController issuing READ command\n";
                commandConnection->issue({BankCommand::Opcode::READ,
                                          currentCommand.getAddress(),
                                          currentCommand.getData()});
                break;
            case MemoryCommand::Opcode::WRITE:
                Logging::log() << Logging::LogLevel::EVENT
                               << "MemoryController issuing WRITE command\n";
                commandConnection->issue({BankCommand::Opcode::WRITE,
                                          currentCommand.getAddress(),
                                          currentCommand.getData()});
                break;
            case MemoryCommand::Opcode::NO_OP:
                break;
        }
    }
    working = false;
}

NVM::Stats::StatBlock NVM::ComponentType::MemoryController::getStats() {
    return Stats::StatBlock();
}

bool NVM::ComponentType::MemoryController::issue(MemoryCommand command) {
    if (working) return false;
    working = true;
    currentCommand = command;
    return true;
}

void NVM::ComponentType::MemoryController::setCommandConnection(
    Connection<BankCommand>* connection) {
    commandConnection = connection;
}

bool NVM::ComponentType::MemoryController::busy() const { return working; }
