#include "MemoryTypes/Component/SimpleBankTimingRules.h"

#include "Logging/Logging.h"
#include "MemoryTypes/Component/Bank/BankCommand.h"

using namespace NVM::Logging;

NVM::ComponentType::SimpleBankTimingRules::SimpleBankTimingRules() :
    currentCycle(0),
    lastCommand(BankCommand::Opcode::NO_OP) {
    nextCycle[BankCommand::Opcode::NO_OP] = 0;
    nextCycle[BankCommand::Opcode::READ] = 0;
    nextCycle[BankCommand::Opcode::WRITE] = 0;
    nextCycle[BankCommand::Opcode::PRECHARGE] = 0;
    nextCycle[BankCommand::Opcode::ACTIVATE] = 0;

    delays[{BankCommand::Opcode::NO_OP, BankCommand::Opcode::NO_OP}] = 1;
    delays[{BankCommand::Opcode::NO_OP, BankCommand::Opcode::READ}] = 1;
    delays[{BankCommand::Opcode::NO_OP, BankCommand::Opcode::WRITE}] = 1;
    delays[{BankCommand::Opcode::NO_OP, BankCommand::Opcode::PRECHARGE}] = 1;
    delays[{BankCommand::Opcode::NO_OP, BankCommand::Opcode::ACTIVATE}] = 1;

    delays[{BankCommand::Opcode::READ, BankCommand::Opcode::NO_OP}] = 1;
    delays[{BankCommand::Opcode::READ, BankCommand::Opcode::READ}] = 5;
    delays[{BankCommand::Opcode::READ, BankCommand::Opcode::WRITE}] = 5;
    delays[{BankCommand::Opcode::READ, BankCommand::Opcode::PRECHARGE}] = 5;
    delays[{BankCommand::Opcode::READ, BankCommand::Opcode::ACTIVATE}] = -1;

    delays[{BankCommand::Opcode::WRITE, BankCommand::Opcode::NO_OP}] = 1;
    delays[{BankCommand::Opcode::WRITE, BankCommand::Opcode::READ}] = 5;
    delays[{BankCommand::Opcode::WRITE, BankCommand::Opcode::WRITE}] = 5;
    delays[{BankCommand::Opcode::WRITE, BankCommand::Opcode::PRECHARGE}] = 5;
    delays[{BankCommand::Opcode::WRITE, BankCommand::Opcode::ACTIVATE}] = -1;

    delays[{BankCommand::Opcode::PRECHARGE, BankCommand::Opcode::NO_OP}] = 1;
    delays[{BankCommand::Opcode::PRECHARGE, BankCommand::Opcode::READ}] = -1;
    delays[{BankCommand::Opcode::PRECHARGE, BankCommand::Opcode::WRITE}] = -1;
    delays[{BankCommand::Opcode::PRECHARGE, BankCommand::Opcode::PRECHARGE}] =
        -1;
    delays[{BankCommand::Opcode::PRECHARGE, BankCommand::Opcode::ACTIVATE}] = 5;

    delays[{BankCommand::Opcode::ACTIVATE, BankCommand::Opcode::NO_OP}] = 1;
    delays[{BankCommand::Opcode::ACTIVATE, BankCommand::Opcode::READ}] = 5;
    delays[{BankCommand::Opcode::ACTIVATE, BankCommand::Opcode::WRITE}] = 5;
    delays[{BankCommand::Opcode::ACTIVATE, BankCommand::Opcode::PRECHARGE}] = 5;
    delays[{BankCommand::Opcode::ACTIVATE, BankCommand::Opcode::ACTIVATE}] = -1;
}

void NVM::ComponentType::SimpleBankTimingRules::issue(BankCommand cmd) {
    if (!validate(cmd)) log() << LogLevel::ERROR << "Bank timing error!\n";
    updateNextCycle(BankCommand::Opcode::NO_OP, cmd);
    updateNextCycle(BankCommand::Opcode::READ, cmd);
    updateNextCycle(BankCommand::Opcode::WRITE, cmd);
    updateNextCycle(BankCommand::Opcode::ACTIVATE, cmd);
    updateNextCycle(BankCommand::Opcode::PRECHARGE, cmd);
}

bool NVM::ComponentType::SimpleBankTimingRules::validate(BankCommand cmd) {
    return nextCycle[cmd.getOpcode()] <= currentCycle &&
           nextCycle[cmd.getOpcode()] >= 0;
}

void NVM::ComponentType::SimpleBankTimingRules::cycle() { currentCycle++; }

void NVM::ComponentType::SimpleBankTimingRules::updateNextCycle(
    BankCommand::Opcode opcode, BankCommand cmd) {
    if (delays[{cmd.getOpcode(), opcode}] < 0) nextCycle[opcode] = -1;
    else nextCycle[opcode] = currentCycle + delays[{cmd.getOpcode(), opcode}];
}
