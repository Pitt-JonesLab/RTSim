#pragma once

#include "MemoryTypes/Component/BankState.h"

namespace NVM::ComponentType {

class ReadingState : public BankState {
    public:
    ReadingState(Connection<BankCommand>* cmd,
                 Connection<BankResponse>* response);

    void process();
    void cycle();

    std::unique_ptr<BankState> nextState();
};

} // namespace NVM::ComponentType