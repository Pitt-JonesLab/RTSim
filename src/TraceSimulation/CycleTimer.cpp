#include "src/TraceSimulation/CycleTimer.h"

#include <algorithm>

using namespace NVM::Simulation;

CycleTimer::CycleTimer(unsigned int max) : currentCycle(0), maxCycles(max) {}

unsigned int CycleTimer::cycle(unsigned int cycles) {
    auto realCycles = std::min(maxCycles - currentCycle, cycles);
    realCycles = (maxCycles == 0) ? cycles : realCycles;
    currentCycle += realCycles;
    return realCycles;
}

unsigned int CycleTimer::getCurrentCycle() const { return currentCycle; }

unsigned int CycleTimer::getMaxCycle() const { return maxCycles; }