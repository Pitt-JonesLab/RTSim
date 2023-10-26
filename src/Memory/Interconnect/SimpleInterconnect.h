#pragma once

#include "Memory/Interconnect.h"

#include <vector>

namespace NVM::Memory {

class SimpleInterconnect : public Interconnect {
    public:
    SimpleInterconnect();

    Command* read(uint64_t address, NVM::Simulation::DataBlock data);

    Command* write(uint64_t address, NVM::Simulation::DataBlock data);

    Command* rowClone(uint64_t srcAddress, uint64_t destAddress,
                      NVM::Simulation::DataBlock data);
    Command* transverseRead(uint64_t baseAddress, uint64_t destAddress,
                            std::vector<NVM::Simulation::DataBlock> data);
    Command* transverseWrite(uint64_t address,
                             std::vector<NVM::Simulation::DataBlock> data);

    void cycle(unsigned int cycles);

    bool isEmpty() const;

    void addRank(std::unique_ptr<Rank> rank);

    StatBlock getStats(std::string tag) const;

    private:
    unsigned int totalReads, totalWrites;

    std::vector<std::unique_ptr<Rank>> ranks;
    std::unique_ptr<Command> currentCommand;
};

} // namespace NVM::Memory