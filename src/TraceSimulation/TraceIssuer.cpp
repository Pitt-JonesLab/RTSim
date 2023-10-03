#include "src/TraceSimulation/TraceIssuer.h"

using namespace NVM::Simulation;

TraceIssuer::TraceIssuer(std::unique_ptr<TraceReader> reader,
                         unsigned int cycles) :
    reqMaker(std::move(reader)),
    timer(cycles) {}

bool TraceIssuer::issue(MemorySystem* memory) { return false; }