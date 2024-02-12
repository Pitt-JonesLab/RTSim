#include "MemoryTypes/Component/Bank/BankState.h"

NVM::ComponentType::BankState::BankState(Connection<BankCommand>* cmd,
                                         Connection<BankResponse>* response) :
    commandConnection(cmd),
    responseConnection(response),
    State({}) {}
