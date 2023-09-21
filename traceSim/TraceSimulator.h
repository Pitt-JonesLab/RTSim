#pragma once

#include "include/NVMTypes.h"

#include <cstdint>
#include <iostream>

namespace NVM {

class MemoryComponent;
class GenericTraceReader;
class NVMainRequest;
class TraceLine;

class TraceMain {
    public:
    /**
     * Creates a TraceMain object
     */
    TraceMain(MemoryComponent* memory, bool ignoreData, bool ignoreReqCycle,
              GenericTraceReader* traceReader, ncycle_t maxCycles,
              std::ostream& statStream);

    /**
     * Simulates the given trace file
     *
     * @return 0 is the trace runs successfully, nonzero otherwise
     */
    int runTrace();

    private:
    MemoryComponent* memorySystem;
    bool ignoreData, ignoreReqCycle;
    GenericTraceReader* trace;
    std::ostream& statStream;
    uint64_t simulateCycles, currentCycle;

    /**
     * Performs the trace simulation
     */
    void runSimulation();

    /**
     * Prints simulation stats to std output
     */
    void printStats();

    NVMainRequest* getNextRequest();

    void waitForDrain();

    bool issueRequest(NVMainRequest* req);

    bool issueRowClone(NVMainRequest* req);

    bool tryCycle(ncycle_t cycles);
};

} // namespace NVM