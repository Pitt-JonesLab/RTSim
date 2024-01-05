#pragma once

#include "Utils/Address.h"

#include <cstdint>

namespace NVM {

// using Address = uint64_t;

enum class CommandType { READ = 0, WRITE, ROWCLONE, PIM, ACTIVATE, PRECHARGE };

const unsigned int numCommandTypes = 6;

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