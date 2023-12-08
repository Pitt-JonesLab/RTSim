#include "SimpleTimer.h"

using namespace NVM::Timing;

SimpleTimer::SimpleTimer() : remainingCycles(0) {}

void SimpleTimer::cycle() {
    if (remainingCycles) remainingCycles--;
}

bool SimpleTimer::isAvailable(Address address) const {
    return remainingCycles == 0;
}

void SimpleTimer::issue(Command cmd) { remainingCycles = 5; }