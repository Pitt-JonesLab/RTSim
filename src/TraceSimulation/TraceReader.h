#pragma once

#include <string>

namespace NVM::Simulation {

enum class Opcode1 { READ, WRITE, PIM, TRANSVERSE_WRITE, SHIFT, NONE };

enum class Opcode2 {

};

/**
 * Data from each trace line
 */
struct TraceLine {
    unsigned int cycle;
    Opcode1 op1;
    Opcode2 op2;
    // TODO make Address class? Use NVMAddress?
    unsigned int threadId;

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
    /**
     * Gets the next TraceLine
     *
     * @return Next TraceLine in source. Returns NONE type TraceLine if there
     * are no more lines to read.
     */
    virtual TraceLine getLine() = 0;
};

} // namespace NVM::Simulation