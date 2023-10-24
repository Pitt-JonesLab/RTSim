#include "Memory/NullCommand.h"

using namespace NVM::Memory;

NullCommand nullCmd;

Command* NVM::Memory::sendNull() {
    return &nullCmd;
}

NullCommand::NullCommand() {}

void NullCommand::setParent(Command* parent) { parent->notify(); }

void NullCommand::notify() {}

bool NullCommand::isDone() const { return true; }

void NullCommand::cycle(unsigned int cycles) {}