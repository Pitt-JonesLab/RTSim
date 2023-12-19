#include "SimpleTimer.h"

using namespace NVM::Timing;

SimpleTimer::SimpleTimer() : remainingCycles(0) {}

void SimpleTimer::cycle() {
    if (remainingCycles) remainingCycles--;
}

bool SimpleTimer::busy() const { return remainingCycles > 0; }

void SimpleTimer::issue(Command cmd, unsigned int numTries) {
    remainingCycles = 5 * numTries;
}

void SimpleTimer::addDelay(unsigned int delayCycles) {
    remainingCycles += delayCycles;
}

void SimpleTimer::addFaultDelay(Command cmd) { remainingCycles += 5; }