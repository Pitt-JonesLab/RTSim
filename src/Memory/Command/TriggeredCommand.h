#pragma once

#include "Memory/Command/Command.h"

#include <functional>
#include <memory>

namespace NVM::Memory {

/**
 * Command with manual start
 * Wraps another Command given by a callback
 */
class TriggeredCommand : public Command {
    private:
    Command *parent, *cmd;
    std::function<Command*()> cmdFunc;
    bool done;

    public:
    TriggeredCommand(std::function<Command*()> cmdFunc);

    void setParent(Command* parent);

    void notify();

    bool isDone() const;

    void cycle(unsigned int);

    /**
     * Creates the child Command
     */
    void issue();

    /**
     * True if child Command is created, false otherwise
     */
    bool triggered() const;
};

} // namespace NVM::Memory