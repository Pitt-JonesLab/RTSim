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

    NVM::Memory::Command getNextCommand() {
        if (commands.empty()) return NVM::Memory::Command();
        auto command = std::move(commands.front());
        commands.pop();
        return command;
    }

    void addCommand(NVM::Memory::Command command) { commands.push(command); }

    std::queue<NVM::Memory::Command> commands;
};

} // namespace NVM::Simulation