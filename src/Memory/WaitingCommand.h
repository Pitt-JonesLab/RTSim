#pragma once

#include "Memory/Command.h"

namespace NVM::Memory {

/*
*   Waits for child Command to complete
*/
class WaitingCommand : public Command {
    public:
        WaitingCommand();

        void setParent(Command* p);
        void notify();
        bool isDone() const;
        void cycle(unsigned int);

    private:
        bool completed;
        Command* parent;
};

}