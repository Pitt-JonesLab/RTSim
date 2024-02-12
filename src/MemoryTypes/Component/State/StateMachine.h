#pragma once

#include "MemoryTypes/Component/Component.h"
#include "MemoryTypes/Component/State/State.h"
#include "Stats/StatBlock.h"

#include <memory>

namespace NVM::ComponentType {

class StateMachine : public Component {
    public:
    StateMachine(std::unique_ptr<StateWrapper> firstState) :
        state(std::move(firstState)) {}

    void process() { state->process(); }

    void cycle() {
        state->cycle();
        auto nextState = state->getNext();
        if (nextState) {
            auto s = state->getStats();
            stats += s;
            state = std::move(nextState);
        }
    }

    Stats::ValueStatBlock getStats(std::string tag) {
        auto s = state->getStats();
        stats += s;
        stats.setTag(tag);
        return stats;
    }

    protected:
    std::unique_ptr<StateWrapper> state;
    Stats::ValueStatBlock stats;
};

} // namespace NVM::ComponentType