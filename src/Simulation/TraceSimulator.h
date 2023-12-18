#pragma once

#include "Simulation/TraceIssuer.h"
#include "Memory/Event/MemoryEventQueue.h"

#include <memory>

namespace NVM::Simulation {

using NVM::Memory::MemoryEventQueue;

/**
 * Given a TraceIssuer and a MemorySystem, simulates the trace to completion
 */
class TraceSimulator {
    public:
    /**
     * Creates a TraceSimulator with a given TraceReader and MemorySystem
     *
     * @param reader TraceReader for this simulator
     * @param receiver Commandable to be simulated
     * @param maxCycles Maximum number of cycles to simulate. 0 means no max.
     */
    TraceSimulator(std::unique_ptr<TraceReader> reader,
                   std::unique_ptr<Commandable> receiver,
                   unsigned int maxCycles = 0);

    /**
     * Simulates the trace with the MemorySystem. Completes when the maximum
     * cycle has been reached or when all requests have been completed.
     */
    void run();

    /**
     * Prints results of simulation
     *
     * @param statStream Output stream for stats
     */
    void printStats(std::ostream& statStream);

    private:
    MemoryEventQueue queue;
    std::unique_ptr<Commandable> receiver;
    CycleTimer timer;
};

} // namespace NVM::Simulation