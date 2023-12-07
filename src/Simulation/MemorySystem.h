#pragma once

#include "Simulation/DataBlock.h"

#include <iostream>
#include <vector>

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

    virtual bool read(uint64_t address, DataBlock data, unsigned int threadId,
                      unsigned int cycle) = 0;
    virtual bool write(uint64_t address, DataBlock data, unsigned int threadId,
                       unsigned int cycle) = 0;

    virtual bool rowClone(uint64_t srcAddress, uint64_t destAddress,
                          DataBlock data, unsigned int threadId,
                          unsigned int cycle) = 0;

    virtual bool transverseRead(uint64_t baseAddress, uint64_t destAddress,
                                std::vector<DataBlock> inputRows,
                                unsigned int threadId, unsigned int cycle) = 0;

    virtual bool transverseWrite(uint64_t baseAddress,
                                 std::vector<DataBlock> writeData,
                                 unsigned int threadId, unsigned int cycle) = 0;

    virtual bool shift(uint64_t address, unsigned int shiftAmount,
                       DataBlock data, unsigned int threadId,
                       unsigned int cycle) = 0;

    /**
     * Cycles the memory
     *
     * @param cycles Number of cycles to advance
     */
    virtual void cycle(unsigned int cycles) = 0;

    virtual unsigned int getCurrentCycle() = 0;

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

    virtual void failNext() = 0;
};

} // namespace NVM::Simulation