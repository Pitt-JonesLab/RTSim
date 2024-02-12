#pragma once

#include "MemoryTypes/Component/BankCommand.h"
#include "MemoryTypes/Component/BankResponse.h"
#include "MemoryTypes/Component/Connection.h"
#include "MemoryTypes/Component/State.h"
#include "Stats/StatBlock.h"

#include <variant>

namespace NVM::ComponentType {

class BankState : public State<std::monostate> {
    public:
    BankState(Connection<BankCommand>* cmd, Connection<BankResponse>* response);

    virtual bool busy() const = 0;

    protected:
    Connection<BankCommand>* commandConnection;
    Connection<BankResponse>* responseConnection;
};

} // namespace NVM::ComponentType