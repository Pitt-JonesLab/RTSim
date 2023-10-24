#include "Memory/NullCommand.h"

NullCommand() {}

void setParent(Command* parent) { parent->notify(); }

void notify() {}

bool isDone() const { return true; }