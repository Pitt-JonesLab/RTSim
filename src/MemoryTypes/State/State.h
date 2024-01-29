#pragma once

#include "MemoryTypes/State/Commandable.h"

#include <memory>

namespace NVM::State {

/*
 *   Represents the state of a memory device
 */
class State : public Commandable {
    public:
    /*
     *   Progresses the State forward by 1 cycle.
     *
     *   @return Next State
     */
    virtual std::unique_ptr<State> cycle() = 0;
};

} // namespace NVM::State