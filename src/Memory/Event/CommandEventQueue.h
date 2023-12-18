#pragma once

#include "Event/EventQueue.h"

#include <queue>

namespace NVM::Memory {

template <typename Receiver>
class CommandEventQueue : public NVM::Event::EventQueue {
    public:
    using Cmd = std::function<bool(Receiver&)>;

    CommandEventQueue(Receiver* receiver) : receiver(receiver) {}

    bool issue() {
        if (commands.empty()) return false;

        if (commands.front()(*receiver)) {
            commands.pop();
            return true;
        }
        return false;
    }

    void enqueue(Cmd cmd) {
        commands.push(cmd);
    }

    bool isEmpty() const { return commands.empty(); }

    private:
    Receiver* receiver;
    std::queue<Cmd> commands;
};

}