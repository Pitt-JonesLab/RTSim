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

    std::unique_ptr<TraceCommand> getNext() {
        if (commands.empty()) return nullptr;
        auto command = std::move(commands.front());
        commands.pop();
        return std::move(command);
    }

    void addCommand(std::unique_ptr<TraceCommand> command) {
        commands.push(std::move(command));
    }

    std::queue<std::unique_ptr<TraceCommand>> commands;
};

} // namespace NVM::Simulation