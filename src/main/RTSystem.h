#pragma once

#include "src/NVMObject.h"
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
    void cycle(unsigned int cycles);
    bool isEmpty() const;

    bool RequestComplete(NVMainRequest* req);

    void printStats(std::ostream& statStream);

    unsigned int getCurrentCycle();

    private:
    unsigned int outstandingRequests;

    NVMainRequest *nextRead, *nextWrite;
};

} // namespace NVM::Main