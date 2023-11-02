#include "Memory/Command/WaitingCommand.h"

using namespace NVM::Memory;

WaitingCommand::WaitingCommand() : parent(nullptr), completed(false) {}

WaitingCommand::WaitingCommand(Command* child) : WaitingCommand() {
    child->setParent(this);
}

void WaitingCommand::setParent(Command* p) { parent = p; }

void WaitingCommand::notify() {
    if (parent) parent->notify();
    completed = true;
}

bool WaitingCommand::isDone() const { return completed; }

void WaitingCommand::cycle(unsigned int) {}