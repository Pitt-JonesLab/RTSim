#pragma once

#include "Simulation/Command/TraceCommand.h"
#include "Simulation/Commandable.h"
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
     * Gets the next TraceCommand
     *
     * @return Next TraceCommand from the source. Returns nullptr if there are
     * no more commands remaining.
     */
    virtual std::unique_ptr<TraceCommand> getNext();

    Command getNextCommand();

    private:
    std::istream& trace;
};

} // namespace NVM::Simulation