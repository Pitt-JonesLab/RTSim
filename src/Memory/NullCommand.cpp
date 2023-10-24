#include "Memory/NullCommand.h"

using namespace NVM::Memory;

NullCommand::NullCommand() {}

void NullCommand::setParent(Command* parent) { parent->notify(); }

void NullCommand::notify() {}

bool NullCommand::isDone() const { return true; }