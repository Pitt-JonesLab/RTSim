#include "MemoryTypes/Component/BankState.h"

NVM::ComponentType::BankState::BankState(Connection<BankCommand>* cmd,
                                         Connection<BankResponse>* response) :
    commandConnection(cmd),
    responseConnection(response),
    next(nullptr) {}
