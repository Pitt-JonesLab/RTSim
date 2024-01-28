#pragma once

#include "MemoryTypes/Simple/Interconnect.h"

#include <memory>

namespace NVM::Memory {

class MemoryController {
    public:
    virtual bool read(uint64_t address, NVM::Simulation::DataBlock data) = 0;
    virtual bool write(uint64_t address, NVM::Simulation::DataBlock data) = 0;

    virtual bool rowClone(uint64_t srcAddress, uint64_t destAddress,
                          NVM::Simulation::DataBlock data) = 0;

    virtual bool
    transverseRead(uint64_t baseAddress, uint64_t destAddress,
                   std::vector<NVM::Simulation::DataBlock> data) = 0;

    virtual bool
    transverseWrite(uint64_t address,
                    std::vector<NVM::Simulation::DataBlock> data) = 0;

    virtual bool isEmpty() const = 0;

    virtual void failNext() = 0;

    virtual void cycle(unsigned int cycles) = 0;

    virtual NVM::Stats::StatBlock getStats(std::string tag) const = 0;

    void addInterconnect(std::unique_ptr<Interconnect> interconnect);

    protected:
    std::vector<std::unique_ptr<Interconnect>> interconnects;
};

} // namespace NVM::Memory