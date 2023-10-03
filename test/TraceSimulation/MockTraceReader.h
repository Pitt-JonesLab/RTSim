#pragma once

#include "src/TraceSimulation/TraceReader.h"

#include <queue>

namespace NVM::Simulation {

/**
 * Mock trace reader that simply pulls from a queue of given TraceLines.
 */
class MockTraceReader : public TraceReader {
    public:
    MockTraceReader() {}

    TraceLine getLine() {
        if (lines.empty()) return TraceLine();
        auto line = lines.front();
        lines.pop();
        return line;
    }

    void addLine(TraceLine line) { lines.push(line); }

    private:
    std::queue<TraceLine> lines;
};

} // namespace NVM::Simulation