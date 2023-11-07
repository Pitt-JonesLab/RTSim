#pragma once

#include "Memory/Command/TriggeredCommand.h"
#include "Memory/MemoryController/MemoryController.h"

#include <queue>
#include <vector>

namespace NVM::Memory {

class SimpleController : public MemoryController {
    public:
    SimpleController();

    bool read(uint64_t address, NVM::Simulation::DataBlock data);
    bool write(uint64_t address, NVM::Simulation::DataBlock data);
    bool rowClone(uint64_t srcAddress, uint64_t destAddress,
                      NVM::Simulation::DataBlock data);
    bool transverseRead(uint64_t baseAddress, uint64_t destAddress,
                            std::vector<NVM::Simulation::DataBlock> data);
    bool transverseWrite(uint64_t address,
                             std::vector<NVM::Simulation::DataBlock> data);

    void cycle(unsigned int cycles);

    bool isEmpty() const;

    void addInterconnect(std::unique_ptr<Interconnect> interconnect);

    StatBlock getStats(std::string tag) const;

    private:
    unsigned int totalReads, totalWrites;

    std::vector<std::unique_ptr<Interconnect>> interconnects;
    std::unique_ptr<Command> currentCommand;

    using CommandQueue = std::queue<std::unique_ptr<TriggeredCommand>>;
    std::unique_ptr<TriggeredCommand> systemCmd;
    std::vector<CommandQueue> bankQueues;

    void issueFromQueue();
};

} // namespace NVM::Memory