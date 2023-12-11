#include "Simulation/TraceSimulator.h"

using namespace NVM::Simulation;

TraceSimulator::TraceSimulator(std::unique_ptr<TraceReader> reader,
                               std::unique_ptr<MemorySystem> memory,
                               unsigned int maxCycles) :
    issuer(std::move(reader), maxCycles),
    memory(std::move(memory)) {}

TraceSimulator::TraceSimulator(std::unique_ptr<TraceReader> reader,
                               std::unique_ptr<Commandable> receiver,
                               unsigned int maxCycles) :
    issuer(std::move(reader), maxCycles),
    receiver(std::move(receiver)) {}

void TraceSimulator::run() {
    if (memory) {
        while (issuer.issue(memory.get())) continue;
        issuer.drain(memory.get());
    }

    if (receiver) {
        while (issuer.issueCommand(*receiver)) continue;
        issuer.drain(*receiver);
    }
}

void TraceSimulator::printStats(std::ostream& statStream) {
    memory->printStats(statStream);
}