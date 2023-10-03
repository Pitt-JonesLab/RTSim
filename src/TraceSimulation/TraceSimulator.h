#pragma once

#include "src/TraceSimulation/MemorySystem.h"
#include "src/TraceSimulation/TraceIssuer.h"

#include <memory>

namespace NVM::Simulation {

/**
 * Given a TraceIssuer and a MemorySystem, simulates the trace to completion
 */
class TraceSimulator {
    public:
    /**
     * Creates a TraceSimulator with a given TraceReader and MemorySystem
     *
     * @param reader TraceReader for this simulator
     * @param memory MemorySystem to be simulated
     */
    TraceSimulator(std::unique_ptr<TraceReader> reader,
                   std::unique_ptr<MemorySystem> memory);

    /**
     * Simulates the trace with the MemorySystem. Completes when the maximum
     * cycle has been reached or when all requests have been completed.
     */
    void run();

    private:
    TraceIssuer issuer;
    std::unique_ptr<MemorySystem> memory;
};

} // namespace NVM::Simulation