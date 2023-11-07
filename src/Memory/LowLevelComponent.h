#pragma once

#include "Memory/Command/Command.h"
#include "Simulation/DataBlock.h"

#include <vector>

namespace NVM::Memory {

class LowLevelComponent {
    public:
    using DataBlock = NVM::Simulation::DataBlock;
    using Address = uint64_t;

    virtual Command* read(Address address, DataBlock data) = 0;
    virtual Command* write(Address address, DataBlock data) = 0;

    virtual Command* rowClone(Address srcAddress, Address destAddress,
                              DataBlock data) = 0;

    virtual Command* transverseRead(Address baseAddress, Address destAddress,
                                    std::vector<DataBlock> data) = 0;

    virtual Command* transverseWrite(Address address,
                                     std::vector<DataBlock> data) = 0;

    virtual Command* activate(Address address) = 0;
    
    virtual Command* precharge() = 0;
    
    virtual Command* refresh() = 0;
};

} // namespace NVM::Memory