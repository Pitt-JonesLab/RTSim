#pragma once

#include "src/TraceSimulation/CycleTimer.h"
#include "src/TraceSimulation/TraceRequestMaker.h"

namespace NVM::Simulation {

class MemorySystem;

/**
 * Attempts to issue requests to a given MemorySystem
 */
class TraceIssuer {
    public:
    /**
     * Creates a TraceIssuer with a given TraceRequestMaker and CycleTimer
     *
     * @param reqMaker TraceRequestMaker for this TraceIssuer
     * @param timer CycleTimer for this TraceIssuer
     */
    TraceIssuer(TraceRequestMaker reqMaker, CycleTimer timer);

    /**
     * Attempts to issue the next request to the given MemorySystem. This will
     * fail if the request maker has run out of requests or the timer has
     * reached the maximum cycle.
     *
     * @param memory MemorySystem to receive the next request
     *
     * @return True if successful, false otherwise
     */
    bool issue(MemorySystem* memory);

    private:
    TraceRequestMaker reqMaker;
    CycleTimer timer;
};

} // namespace NVM::Simulation