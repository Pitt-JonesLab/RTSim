#pragma once

#include "src/NVMObject.h"
#include "src/TraceSimulation/MemorySystem.h"

namespace NVM::Main {

class RTSystem : public NVM::Simulation::MemorySystem, NVM::NVMObject {
    public:
    RTSystem(NVM::Config* config);

    bool issue(NVMainRequest* req);
    void cycle(unsigned int cycles);
    bool isEmpty() const;

    bool RequestComplete(NVMainRequest* req);

    void printStats(std::ostream& statStream);

    private:
    unsigned int outstandingRequests;
};

} // namespace NVM::Main