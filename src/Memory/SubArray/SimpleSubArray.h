#pragma once

#include "Memory/SubArray/RowController.h"
#include "Memory/SubArray/SubArray.h"

#include <memory>

namespace NVM::Memory {

class SimpleSubArray : public SubArray {
    public:
    SimpleSubArray(unsigned int rows);

    Command* read(uint64_t address, NVM::Simulation::DataBlock data);
    Command* write(uint64_t address, NVM::Simulation::DataBlock data);
    Command* switchRow(unsigned int row);

    void cycle(unsigned int cycles);

    bool isEmpty() const;

    StatBlock getStats(std::string tag) const;

    private:
    unsigned int totalReads;
    unsigned int totalWrites;
    unsigned int totalActivates;
    unsigned int totalPrecharges;
    unsigned int rowBufferHits;

    std::unique_ptr<Command> currentCommand;

    RowController rowControl;
};

} // namespace NVM::Memory