#include "src/TraceSimulation/TraceSimulator.h"

using namespace NVM::Simulation;

TraceSimulator::TraceSimulator(std::unique_ptr<TraceReader> reader,
                               std::unique_ptr<MemorySystem> memory) :
    issuer(std::move(reader), 0),
    memory(std::move(memory)) {}

void TraceSimulator::run() {}