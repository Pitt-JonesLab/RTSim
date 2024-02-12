#pragma once

#include "MemoryTypes/Component/Bank/BankCommand.h"
#include "MemoryTypes/Component/Bank/BankResponse.h"
#include "MemoryTypes/Component/Connection.h"
#include "MemoryTypes/Component/State/State.h"
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