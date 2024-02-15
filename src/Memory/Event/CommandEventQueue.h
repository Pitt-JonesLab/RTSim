#pragma once

#include "Utils/ConfigParser.h"

#include <functional>
#include <queue>
#include <vector>

namespace NVM::Memory {

template<typename Receiver> class CommandEventQueue {
    public:
    using Cmd = std::function<bool(Receiver&)>;

    CommandEventQueue(Receiver* receiver) : receiver(receiver) {
        ConfigParser::registerValue("NumTries", 1, &numTries);

        nextCmd = commands.begin();
    }

    bool issue() {
        if (nextCmd == commands.end()) return false;

        if ((*nextCmd)(*receiver)) {
            nextCmd++;
            if (nextCmd == commands.end() && numTries > 1) {
                numTries--;
                nextCmd = commands.begin();
            }
            return true;
        }
        return false;
    }

    void enqueue(Cmd cmd) {
        commands.push_back(cmd);
        nextCmd = commands.begin();
    }

    bool isEmpty() const { return nextCmd == commands.end(); }

    private:
    Receiver* receiver;
    std::vector<Cmd> commands;

    int numTries;
    typename std::vector<Cmd>::iterator nextCmd;
};

} // namespace NVM::Memory