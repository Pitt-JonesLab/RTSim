#pragma once

#include "Memory/MemoryController.h"

#include <vector>

namespace NVM::Memory {

class SimpleController : public MemoryController {
    public:
    Command* read(uint64_t address, NVM::Simulation::DataBlock data);
    Command* write(uint64_t address, NVM::Simulation::DataBlock data);

    void cycle(unsigned int cycles);

    bool isEmpty() const;

    void addInterconnect(std::unique_ptr<Interconnect> interconnect);

    private:
    std::vector<std::unique_ptr<Interconnect>> interconnects;
    std::unique_ptr<Command> currentCommand;
};

} // namespace NVM::Memory