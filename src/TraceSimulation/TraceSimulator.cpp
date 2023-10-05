#include "src/TraceSimulation/TraceSimulator.h"

using namespace NVM::Simulation;

TraceSimulator::TraceSimulator(std::unique_ptr<TraceReader> reader,
                               std::unique_ptr<MemorySystem> memory,
                               unsigned int maxCycles) :
    issuer(std::move(reader), maxCycles),
    memory(std::move(memory)) {}

void TraceSimulator::run() {
    while (issuer.issue(memory.get())) continue;
    issuer.drain(memory.get());
}

void TraceSimulator::printStats(std::ostream& statStream) {
    memory->printStats(statStream);
}