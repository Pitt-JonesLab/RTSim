#include "Memory/Command/TriggeredCommand.h"

using namespace NVM::Memory;

TriggeredCommand::TriggeredCommand(std::function<Command*()> func) :
    parent(nullptr),
    cmd(nullptr),
    cmdFunc(func),
    done(false) {}

void TriggeredCommand::setParent(Command* p) { parent = p; }

void TriggeredCommand::notify() {
    if (parent) parent->notify();
    parent = nullptr;
    cmd = nullptr;
    done = true;
}

bool TriggeredCommand::isDone() const { return done; }

void TriggeredCommand::cycle(unsigned int cycles) {
    if (cmd) cmd->cycle(cycles);
}

void TriggeredCommand::issue() {
    if (cmd) return;
    cmd = cmdFunc();
    if (cmd) cmd->setParent(this);
}

bool TriggeredCommand::triggered() const { return cmd; }