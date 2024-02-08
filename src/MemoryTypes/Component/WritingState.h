#pragma once

#include "MemoryTypes/Component/BankState.h"

namespace NVM::ComponentType {

class WritingState : public BankState {
    public:
    WritingState(Connection<BankCommand>* cmd,
                 Connection<BankResponse>* response);

    void process();
    void cycle();

    std::unique_ptr<BankState> nextState();
};

} // namespace NVM::ComponentType