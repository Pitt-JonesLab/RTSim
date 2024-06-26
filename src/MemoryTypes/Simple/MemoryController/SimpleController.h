#pragma once

#include "MemoryTypes/Simple/MemoryController.h"
#include "MemoryTypes/Simple/Modeling/System/SimpleSystem.h"
#include "MemoryTypes/Simple/Parsing/Parser/SimpleParser.h"
#include "MemoryTypes/Simple/Scheduling/Scheduler/RBScheduler.h"

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
    bool transverseWrite(uint64_t address, NVM::Simulation::DataBlock data);

    void cycle(unsigned int cycles);

    bool isEmpty() const;

    void failNext();

    NVM::Stats::StatBlock getStats(std::string tag) const;

    private:
    unsigned int rowBufferHits;
    bool fails;

    NVM::Scheduling::Instruction rInst;
    bool received;

    NVM::Parsing::SimpleParser parser;
    NVM::Scheduling::RBScheduler scheduler;
    NVM::Modeling::SimpleSystem model;

    std::vector<NVM::Command> cmdQueue;
};

} // namespace NVM::Memory