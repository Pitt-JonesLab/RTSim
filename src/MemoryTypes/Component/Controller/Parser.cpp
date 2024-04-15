#include "MemoryTypes/Component/Controller/Parser.h"

#include "MemoryTypes/Component/Bank/BankCommand.h"
#include "MemoryTypes/Component/System/MemoryCommand.h"
#include "MemoryTypes/Simple/Modeling/Decoder.h"

using namespace NVM::ComponentType;
using namespace NVM::Modeling;

std::vector<BankCommand> NVM::ComponentType::Parser::parse(MemoryCommand cmd,
                                                           BankModel& bank) {
    auto bankRow = bank.getOpenRow();
    auto cmdRow = decodeSymbol(AddressSymbol::ROW, cmd.getAddress().getData());

    std::vector<BankCommand> bankCommands;

    if (bankRow != cmdRow) {
        if (bankRow >= 0)
            bankCommands.push_back(
                {BankCommand::Opcode::PRECHARGE, 0, RowData()});
        bankCommands.push_back(
            {BankCommand::Opcode::ACTIVATE, cmd.getAddress(), RowData()});
    }
    bank.setOpenRow(cmdRow);

    switch (cmd.getOpcode()) {
        case MemoryCommand::Opcode::READ:
            bankCommands.push_back(
                {BankCommand::Opcode::READ, cmd.getAddress(), cmd.getData()});
            break;
        case MemoryCommand::Opcode::WRITE:
            bankCommands.push_back(
                {BankCommand::Opcode::WRITE, cmd.getAddress(), cmd.getData()});
            break;
        case MemoryCommand::Opcode::ROWCLONE:
            bankCommands.push_back(
                {BankCommand::Opcode::COPY, cmd.getAddress2(), cmd.getData()});
            break;
        case MemoryCommand::Opcode::TRANSVERSE_READ:
            bankCommands.push_back({BankCommand::Opcode::TRANSVERSE_READ,
                                    cmd.getAddress(), cmd.getData()});
            break;
        default:
            break;
    }

    return bankCommands;
}
