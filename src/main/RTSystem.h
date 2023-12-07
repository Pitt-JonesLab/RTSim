#pragma once

#include "src/old/NVMObject.h"
#include "src/Simulation/MemorySystem.h"

namespace NVM::Main {

class RTSystem : public NVM::Simulation::MemorySystem, NVM::NVMObject {
    public:
    RTSystem(NVM::Config* config);

    bool issue(NVMainRequest* req);

    bool read(uint64_t address, NVM::Simulation::DataBlock data,
              unsigned int threadId, unsigned int cycle);
    bool write(uint64_t address, NVM::Simulation::DataBlock data,
               unsigned int threadId, unsigned int cycle);
    bool rowClone(uint64_t srcAddress, uint64_t destAddress,
                  NVM::Simulation::DataBlock data, unsigned int threadId,
                  unsigned int cycle);

    bool transverseRead(uint64_t baseAddress, uint64_t destAddress,
                        std::vector<NVM::Simulation::DataBlock> inputRows,
                        unsigned int threadId, unsigned int cycle);

    bool transverseWrite(uint64_t baseAddress,
                         std::vector<NVM::Simulation::DataBlock> writeData,
                         unsigned int threadId, unsigned int cycle);

    bool shift(uint64_t address, unsigned int shiftAmount,
               NVM::Simulation ::DataBlock data, unsigned int threadId,
               unsigned int cycle);
    void cycle(unsigned int cycles);
    bool isEmpty() const;

    bool RequestComplete(NVMainRequest* req);

    void printStats(std::ostream& statStream);

    unsigned int getCurrentCycle();

    void failNext();

    private:
    unsigned int outstandingRequests;

    NVMainRequest *nextRead, *nextWrite;
};

} // namespace NVM::Main