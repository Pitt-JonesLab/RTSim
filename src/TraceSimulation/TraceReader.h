#pragma once

#include <array>
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

    /**
     * 512-bit data block
     */
    struct DataBlock {
        std::array<uint8_t, 64> bytes;
    };

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
    /**
     * Gets the next TraceLine
     *
     * @return Next TraceLine in source. Returns NONE type TraceLine if there
     * are no more lines to read.
     */
    virtual TraceLine getLine() = 0;

    virtual ~TraceReader() {}
};

} // namespace NVM::Simulation