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
     */
    virtual void cycle() = 0;

    /*
     *   Returns the next State or nullptr if the current State is not
     * completed.
     *
     *  @return Next State or nullptr if the current State is not complete
     */
    virtual std::unique_ptr<State> getNext() const = 0;

    /*
     *  Returns true if this State is complete and has a valid next State.
     *  False otherwise.
     */
    virtual bool finished() const = 0;
};

} // namespace NVM::State