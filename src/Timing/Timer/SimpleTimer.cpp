#include "SimpleTimer.h"

using namespace NVM::Timing;

SimpleTimer::SimpleTimer() : remainingCycles(0) {}

void SimpleTimer::cycle() {
    if (remainingCycles) remainingCycles--;
}

bool SimpleTimer::busy() const {
    return remainingCycles > 0;
}

void SimpleTimer::issue(Command cmd) { remainingCycles = 5; }