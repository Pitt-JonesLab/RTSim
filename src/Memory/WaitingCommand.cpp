#include "Memory/WaitingCommand.h"

using namespace NVM::Memory;

WaitingCommand::WaitingCommand() : parent(nullptr), completed(false) {}

void WaitingCommand::setParent(Command* p) { parent = p; }

void WaitingCommand::notify() {
    if (parent) parent->notify();
    completed = true;
}

bool WaitingCommand::isDone() const { return completed; }