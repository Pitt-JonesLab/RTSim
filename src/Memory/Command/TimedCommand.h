#pragma once

#include "Memory/Command/Command.h"

namespace NVM::Memory {

/*
 *   Simple Command that simply waits for the given number of cycles
 */
class TimedCommand : public Command {
    public:
    TimedCommand(unsigned int cycles = 5);

    void setParent(Command* p);

    void cycle(unsigned int cycles);

    bool isDone() const;

    void notify();

    private:
    Command* parent;
    unsigned int cyclesLeft;
};

} // namespace NVM::Memory