#pragma once

#include "Modeling/Types.h"

namespace NVM::Parsing {

using NVM::Modeling::Address;

enum class CommandType { READ, WRITE, ROWCLONE, PIM, ACTIVATE, PRECHARGE };

class Command {
    public:
    Command(CommandType type, Address address);

    CommandType getType() const;
    Address getAddress() const;

    private:
    CommandType type;
    Address address;
};

} // namespace NVM::Parsing