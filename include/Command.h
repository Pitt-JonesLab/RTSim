#pragma once

#include "Utils/Address.h"

#include <cstdint>

namespace NVM {

// using Address = uint64_t;

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

} // namespace NVM