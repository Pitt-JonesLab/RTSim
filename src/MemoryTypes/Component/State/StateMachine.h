#pragma once

#include "MemoryTypes/Component/Component.h"
#include "MemoryTypes/Component/State/State.h"
#include "Stats/StatBlock.h"

#include <memory>

namespace NVM::ComponentType {

template<typename MachineInfo> class StateMachine : public Component {
    public:
    StateMachine() : state(nullptr) {}

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

    typename MachineInfo::Name getStateName() const { return state->getName(); }

    protected:
    void initialize(std::unique_ptr<State<MachineInfo>> s) {
        state = std::move(s);
    }

    std::unique_ptr<State<MachineInfo>> state;
    Stats::ValueStatBlock stats;

    MachineInfo info;
};

} // namespace NVM::ComponentType