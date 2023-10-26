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
    Command* rowClone(uint64_t srcAddress, uint64_t destAddress,
                      NVM::Simulation::DataBlock data);
    Command* transverseRead(uint64_t baseAddress, uint64_t destAddress,
                            std::vector<NVM::Simulation::DataBlock> data);
    Command* transverseWrite(uint64_t address,
                             std::vector<NVM::Simulation::DataBlock> data);

    void cycle(unsigned int cycles);

    bool isEmpty() const;

    StatBlock getStats(std::string tag) const;

    private:
    unsigned int totalReads;
    unsigned int totalWrites;
    unsigned int totalActivates;
    unsigned int totalPrecharges;
    unsigned int totalShifts;
    unsigned int rowBufferHits;

    double actEnergy, readEnergy, writeEnergy, shiftEnergy;

    std::unique_ptr<Command> currentCommand;

    RowController rowControl;
};

} // namespace NVM::Memory