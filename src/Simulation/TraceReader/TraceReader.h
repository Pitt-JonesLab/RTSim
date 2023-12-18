#pragma once

#include "Memory/Commandable.h"
#include "Simulation/DataBlock.h"

#include <array>
#include <iostream>
#include <memory>
#include <string>
#include <vector>

namespace NVM::Simulation {

/**
 * Reads trace lines from an input source
 */
class TraceReader {
    public:
    TraceReader(std::istream& trace);

    virtual ~TraceReader() {}

    /**
     * Gets the next Command
     *
     * @return Next Command from the source. Returns empty Command if there are
     * no more commands remaining.
     */
    virtual NVM::Memory::Command getNextCommand();

    private:
    std::istream& trace;
};

} // namespace NVM::Simulation