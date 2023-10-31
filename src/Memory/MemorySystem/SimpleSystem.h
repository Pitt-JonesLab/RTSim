#pragma once

#include "Memory/MemorySystem/MemorySystem.h"

#include <vector>

namespace NVM::Memory {

class SimpleSystem : public MemorySystem {
    public:
    SimpleSystem();

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
               NVM::Simulation::DataBlock data, unsigned int threadId,
               unsigned int cycle);

    void cycle(unsigned int cycles);

    unsigned int getCurrentCycle();

    bool isEmpty() const;

    void printStats(std::ostream& statStream);

    void addController(std::unique_ptr<MemoryController> controller);

    private:
    unsigned int totalReads, totalWrites, currentCycle;

    std::vector<std::unique_ptr<MemoryController>> channels;
    std::unique_ptr<Command> currentCommand;
};

} // namespace NVM::Memory