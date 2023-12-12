#pragma once

#include "Simulation/CycleTimer.h"
#include "Simulation/TraceReader/TraceReader.h"

namespace NVM::Simulation {

class MemorySystem;

/**
 * Attempts to issue requests to a given MemorySystem
 */
class TraceIssuer {
    public:
    /**
     * Creates a TraceIssuer with a given TraceReader and maximum number of
     * cycles
     *
     * @param reader TraceReader for this TraceIssuer
     * @param cycles Max number of cycles to simulate
     */
    TraceIssuer(std::unique_ptr<TraceReader> reader, unsigned int cycles);

    /**
     * Attempts to issue the next Command to the given receiver. This will
     * return false if the request maker has run out of requests or the timer
     * has reached the maximum cycle.
     *
     * @param receiver Commandable to receive the next request
     *
     * @return True if successful, false otherwise
     */
    bool issueCommand(Commandable& receiver);

    /**
     * Cycles the Commandable until all requests have been processed
     *
     * @param receiver Commandable to drain
     *
     * @return True if successful, false if timer maxed out
     */
    bool drain(Commandable& receiver);

    private:
    std::unique_ptr<TraceReader> reader;
    CycleTimer timer;
};

} // namespace NVM::Simulation