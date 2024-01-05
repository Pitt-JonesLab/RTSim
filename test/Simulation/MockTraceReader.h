#pragma once

#include "Simulation/TraceReader/TraceReader.h"

#include <queue>

namespace NVM::Simulation {

/**
 * Mock trace reader that simply pulls from a queue of given TraceLines.
 */
class MockTraceReader : public TraceReader {
    public:
    MockTraceReader() : TraceReader(std::cin) {}

    TraceReader::Command getNextCommand() {
        if (commands.empty()) return TraceReader::Command();
        auto command = std::move(commands.front());
        commands.pop();
        return command;
    }

    void addCommand(TraceReader::Command command) { commands.push(command); }

    std::queue<TraceReader::Command> commands;
};

} // namespace NVM::Simulation