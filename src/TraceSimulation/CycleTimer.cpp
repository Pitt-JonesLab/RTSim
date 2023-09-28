#include "src/TraceSimulation/CycleTimer.h"

using namespace NVM::Simulation;

CycleTimer::CycleTimer(unsigned int max) : currentCycle(0), maxCycles(max) {}

bool CycleTimer::cycle(unsigned int cycles) {
    currentCycle += cycles;
    return currentCycle <= maxCycles || maxCycles == 0;
}