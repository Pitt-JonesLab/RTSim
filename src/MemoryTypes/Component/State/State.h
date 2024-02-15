#pragma once

#include "Stats/StatBlock.h"

#include <memory>

namespace NVM::ComponentType {

template<typename StateInfo> class State {
    public:
    State(StateInfo& i) : info(i) {}

    virtual ~State() {}

    std::unique_ptr<State<StateInfo>> getNext() { return std::move(nextState); }

    virtual void process() = 0;
    virtual void cycle() = 0;

    virtual NVM::Stats::ValueStatBlock getStats() = 0;

    virtual bool busy() const = 0;

    protected:
    StateInfo& info;
    std::unique_ptr<State<StateInfo>> nextState;
};

} // namespace NVM::ComponentType
