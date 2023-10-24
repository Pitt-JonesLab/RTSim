#pragma once

#include "Memory/Command.h"

#include <vector>
#include <functional>

namespace NVM::Memory {

/*
*   Handles a chain of commands given as Command producing functions
*   Does not finish until all Commands in the chain are finished
*/
class ChainedCommand : public Command {
    public:
    ChainedCommand(std::vector<std::function<Command*()>>);

    void setParent(Command* parent);

    void notify();

    bool isDone() const;

    bool isBusy() const;
    void getNextStep();

    void cycle(unsigned int);

    private:

    std::vector<std::function<Command*()>> steps;
    Command* parent, *child;
    bool done;
};

}