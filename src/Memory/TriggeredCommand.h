#pragma once

#include "Memory/Command.h"

#include <memory>
#include <functional>

namespace NVM::Memory {

/**
 * Command with manual start
 * Wraps another Command given by a callback
*/
class TriggeredCommand : public Command {
    private:
    std::unique_ptr<Command> cmd;
    Command* parent;
    std::function<Command*()> cmdFunc;

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

}