#include "Timing/Timer/ConfigurableTimer.h"

using namespace NVM::Timing;
using NVM::Command;

ConfigurableTimer::ConfigurableTimer(const NVM::Simulation::Config& conf) :
    remainingCycles(0) {
    timings[static_cast<size_t>(CommandType::READ)] = 5;
    timings[static_cast<size_t>(CommandType::WRITE)] = 5;
    timings[static_cast<size_t>(CommandType::ACTIVATE)] = conf.get<int>("tACT");
    timings[static_cast<size_t>(CommandType::PIM)] = 5;
    timings[static_cast<size_t>(CommandType::ROWCLONE)] = 5;
    timings[static_cast<size_t>(CommandType::PRECHARGE)] = conf.get<int>("tRP");
}

void ConfigurableTimer::cycle() {
    if (remainingCycles) remainingCycles--;
}

bool ConfigurableTimer::busy() const { return remainingCycles > 0; }

void ConfigurableTimer::issue(Command cmd, unsigned int numTries) {
    remainingCycles = timings[static_cast<size_t>(cmd.getType())] * numTries;
}

void ConfigurableTimer::addDelay(unsigned int delayCycles) {
    remainingCycles += delayCycles;
}

void ConfigurableTimer::addFaultDelay(NVM::Command cmd) {
    remainingCycles += timings[static_cast<size_t>(cmd.getType())];
}