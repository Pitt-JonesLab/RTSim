#pragma once

#include "Stats/StatBlock.h"

namespace NVM::ComponentType {

class Component {
    public:
    /*
     *  Processes incoming commands from upstream Components
     */
    virtual void process() = 0;

    /*
     *  Sends outgoing commands to downstream Components
     */
    virtual void cycle() = 0;

    /*
     *  Returns all stat values associated with this Component
     */
    virtual Stats::StatBlock getStats() = 0;
};

} // namespace NVM::ComponentType