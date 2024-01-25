#pragma once

#include "MemoryTypes/Interconnect.h"

namespace NVM::Memory {

class SimpleInterconnect : public Interconnect {
    public:
    void cycle(unsigned int cycles);

    bool isEmpty() const;
};

} // namespace NVM::Memory