#pragma once

#include "Simulation/DataBlock.h"
#include "Simulation/TraceCommand.h"

#include <array>
#include <iostream>
#include <memory>
#include <string>
#include <vector>

namespace NVM::Simulation {

enum class Opcode1 { READ, WRITE, PIM, TRANSVERSE_WRITE, SHIFT, NONE };

enum class Opcode2 { NONE };

/**
 * Data from each trace line
 */
struct TraceLine {
    unsigned int cycle = 0;
    Opcode1 op1 = Opcode1::NONE;
    Opcode2 op2 = Opcode2::NONE;
    uint64_t address1 = 0;
    uint64_t address2 = 0;

    std::vector<DataBlock> data;

    unsigned int threadId = 0;

    /**
     * Allows implicit conversion to bool
     *
     * @return False if TraceLine is NONE type, True otherwise
     */
    operator bool() const { return op1 != Opcode1::NONE; }
};

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

    private:
    std::istream& trace;
};

} // namespace NVM::Simulation