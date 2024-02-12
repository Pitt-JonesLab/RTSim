#pragma once

#include "Stats/StatBlock.h"

#include <memory>

namespace NVM::ComponentType {

class StateWrapper {
    public:
    virtual void process() = 0;
    virtual void cycle() = 0;
    virtual std::unique_ptr<StateWrapper> getNext() = 0;

    virtual NVM::Stats::ValueStatBlock getStats() = 0;

    virtual bool busy() const = 0;
};

template<typename StateInfo> class State : public StateWrapper {
    public:
    State(StateInfo i) : info(i) {}

    std::unique_ptr<StateWrapper> getNext() { return std::move(nextState); }

    protected:
    StateInfo info;
    std::unique_ptr<StateWrapper> nextState;
};

} // namespace NVM::ComponentType
