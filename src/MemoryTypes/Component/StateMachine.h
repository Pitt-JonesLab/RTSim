#pragma once

#include "MemoryTypes/Component/State.h"

#include <memory>

namespace NVM::ComponentType {

class StateMachine {
    public:
    StateMachine(std::unique_ptr<StateWrapper> firstState) :
        state(std::move(firstState)) {}

    void process() { state->process(); }

    void cycle() {
        state->cycle();
        auto nextState = state->getNext();
        if (nextState) state = std::move(nextState);
    }

    private:
    std::unique_ptr<StateWrapper> state;
};

} // namespace NVM::ComponentType