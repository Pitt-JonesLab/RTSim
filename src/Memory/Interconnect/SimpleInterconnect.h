#pragma once

#include "Memory/Interconnect/Interconnect.h"

#include <vector>

namespace NVM::Memory {

class SimpleInterconnect : public Interconnect {
    public:
    void cycle(unsigned int cycles);

    bool isEmpty() const;
};

} // namespace NVM::Memory