#pragma once

#include "Memory/Command.h"

namespace NVM::Memory {

/*
*   Command with nothing to be done.
*   Immediately notifies parent on assignment
*   Notifying this Command does nothing
*/
class NullCommand : public Command{
    public:
    NullCommand();

    void setParent(Command* parent);

    void notify();

    bool isDone() const;

    void cycle(unsigned int cycles);
};

Command* sendNull();

}