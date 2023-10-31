#include "Memory/TriggeredCommand.h"

using namespace NVM::Memory;

TriggeredCommand::TriggeredCommand(std::function<Command*()> func) : parent(nullptr), cmd(nullptr), cmdFunc(func) {}

void TriggeredCommand::setParent(Command* p) { parent = p; }

void TriggeredCommand::notify() {
    if (parent) parent->notify();
}

bool TriggeredCommand::isDone() const {
    if (cmd) return cmd->isDone();
    return false;
}

void TriggeredCommand::cycle(unsigned int cycles) {
    if (cmd) cmd->cycle(cycles);
}

void TriggeredCommand::issue() {
    if (cmd) return; 
    cmd.reset(cmdFunc());
    cmd->setParent(this);
}

bool TriggeredCommand::triggered() const {
    return cmd.get() != nullptr;
}