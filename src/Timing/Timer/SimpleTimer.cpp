#include "SimpleTimer.h"

using namespace NVM::Timing;

SimpleTimer::SimpleTimer() {}

void SimpleTimer::cycle() {}

bool SimpleTimer::isAvailable(Address address) const { return false; }

void SimpleTimer::issue(Command cmd) {}