#pragma once

#include "Memory/MemorySystem/MemorySystem.h"
#include "Memory/Event/CommandEventQueue.h"
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
    using Command = NVM::Memory::CommandEventQueue<NVM::Memory::MemorySystem>::Cmd;

    TraceReader(std::istream& trace);

    virtual ~TraceReader() {}

    /**
     * Gets the next Command
     *
     * @return Next Command from the source. Returns empty Command if there are
     * no more commands remaining.
     */
    virtual Command getNextCommand();

    private:
    std::istream& trace;
};

} // namespace NVM::Simulation