/*******************************************************************************
 * Copyright (c) 2012-2014, The Microsystems Design Labratory (MDL)
 * Department of Computer Science and Engineering, The Pennsylvania State
 *University All rights reserved.
 *
 * This source code is part of NVMain - A cycle accurate timing, bit accurate
 * energy simulator for both volatile (e.g., DRAM) and non-volatile memory
 * (e.g., PCRAM). The source code is free and you can redistribute and/or
 * modify it by providing that the following conditions are met:
 *
 *  1) Redistributions of source code must retain the above copyright notice,
 *     this list of conditions and the following disclaimer.
 *
 *  2) Redistributions in binary form must reproduce the above copyright notice,
 *     this list of conditions and the following disclaimer in the documentation
 *     and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 *AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 *IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
 * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
 * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 * Author list:
 *   Matt Poremba    ( Email: mrp5060 at psu dot edu
 *                     Website: http://www.cse.psu.edu/~poremba/ )
 *   Tao Zhang       ( Email: tzz106 at cse dot psu dot edu
 *                     Website: http://www.cse.psu.edu/~tzz106 )
 *******************************************************************************/

#pragma once

#include "src/Config.h"
#include "src/NVMObject.h"
#include "traceReader/TraceReaderFactory.h"

namespace NVM {

class TraceMain : public NVMObject {
    public:
    /**
     * Creates a TraceMain object
     */
    TraceMain();

    /**
     * Frees memory used by this TraceMain object
     */
    ~TraceMain();

    /**
     * Simulates a given trace file
     *
     * @param argc Number of command line arguments
     * @param argv Array of argument values
     *
     * @return 0 is the trace runs successfully, nonzero otherwise
     */
    int RunTrace(int argc, char* argv[]);

    /**
     * Does nothing
     *
     * @param steps Number of cycles to simulate
     */
    void Cycle(ncycle_t steps);

    /**
     * Completes the given request. Throws an exception if the request is not
     * owned by this TraceMain.
     *
     * @param request Pointer to the request to be completed
     *
     * @return Always true
     */
    bool RequestComplete(NVMainRequest* request);

    private:
    ncounter_t outstandingRequests; // Number of remaining requests
    Config* config;                 // Configuration settings
    bool ignoreData;
    std::ofstream statStream;
    GenericTraceReader* trace;
    uint64_t simulateCycles, currentCycle;

    /**
     * Initializes the configuration settings for the simulation
     */
    void setupConfig(int argc, char* argv[]);

    /**
     * Initializes the child memory module
     */
    void setupChildren();

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

}; // namespace NVM
