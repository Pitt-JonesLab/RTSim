#pragma once

#include <iostream>

namespace NVM {
class NVMainRequest;
}

namespace NVM::Simulation {

/**
 * Interface for Memory Simulation
 */
class MemorySystem {
    public:
    /*
     * Issues a NVMainRequest to the memory
     *
     * @param req Request to be issued
     *
     * @return True if successful, false otherwise
     */
    virtual bool issue(NVMainRequest* req) = 0;

    /**
     * Cycles the memory
     *
     * @param cycles Number of cycles to advance
     */
    virtual void cycle(unsigned int cycles) = 0;

    /**
     * Tells whether any requests are still being processed
     *
     * @return True if there are no requests in memory, false otherwise
     */
    virtual bool isEmpty() const = 0;

    /**
     * Prints stats for the MemorySystem
     *
     * @param statStream Output stream to print stats
     */
    virtual void printStats(std::ostream& statStream) = 0;
};

} // namespace NVM::Simulation