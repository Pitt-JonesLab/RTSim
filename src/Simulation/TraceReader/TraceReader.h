#pragma once

#include "Memory/Event/CommandEventQueue.h"
#include "Memory/MemorySystem/MemorySystem.h"

#include <iostream>

namespace NVM::Simulation {

/**
 * Reads trace lines from an input source
 */
class TraceReader {
    public:
    using Command =
        NVM::Memory::CommandEventQueue<NVM::Memory::MemorySystem>::Cmd;

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