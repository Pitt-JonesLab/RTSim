#include "Parsing/Parser/SimpleParser.h"

#include "Modeling/Decoder.h"

using namespace NVM::Parsing;
using namespace NVM::Modeling;
using namespace NVM::Scheduling;
using namespace NVM;

SimpleParser::SimpleParser() {}

CommandType instToCmd(InstructionType type) {
    switch (type) {
        case InstructionType::READ:
            return CommandType::READ;
        case InstructionType::WRITE:
            return CommandType::WRITE;
        case InstructionType::ROWCLONE:
            return CommandType::ROWCLONE;
        case InstructionType::PIM:
            return CommandType::PIM;
        default:
            return CommandType::READ;
    }
}

std::vector<Command> SimpleParser::parseCommands(const Instruction& transaction,
                                                 MemorySystem& system) {
    std::vector<Command> commands;

    Address address = transaction.getAddress();
    if (system.isOpen(transaction.getAddress())) {
        auto openRow = system.getOpenRow(transaction.getAddress());
        auto instRow =
            decodeSymbol(AddressSymbol::ROW, transaction.getAddress());

        if (openRow != instRow) {
            commands.emplace_back(CommandType::PRECHARGE,
                                  transaction.getAddress());
            system.closeRow(address.getData());
        }
    }

    if (!system.isOpen(transaction.getAddress())) {
        commands.emplace_back(CommandType::ACTIVATE, transaction.getAddress());
        system.openRow(transaction.getAddress());
    }

    commands.push_back(
        Command(instToCmd(transaction.getType()), transaction.getAddress()));

    if (transaction.getType() == InstructionType::PIM ||
        transaction.getType() == InstructionType::ROWCLONE) {
        commands.push_back({CommandType::PRECHARGE, transaction.getAddress()});
        system.closeRow(transaction.getAddress());
    }

    return commands;
}