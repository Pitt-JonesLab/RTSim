#include "Memory/TimedCommand.h"

using namespace NVM::Memory;

TimedCommand::TimedCommand(unsigned int cycles) : parent(nullptr), cyclesLeft(cycles) {}

void TimedCommand::setParent(Command* p) { parent = p; }

void TimedCommand::cycle(unsigned int cycles) {
    if (cycles >= cyclesLeft) {
        cyclesLeft = 0;
        if (parent) parent->notify();
        return;
    }
    cyclesLeft -= cycles;
}

bool TimedCommand::isDone() { return cyclesLeft == 0; }

void TimedCommand::notify() {}