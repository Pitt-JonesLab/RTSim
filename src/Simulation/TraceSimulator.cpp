#include "Simulation/TraceSimulator.h"

using namespace NVM::Simulation;

TraceSimulator::TraceSimulator(std::unique_ptr<TraceReader> reader,
                               std::unique_ptr<Commandable> receiver,
                               unsigned int maxCycles) :
    issuer(std::move(reader), maxCycles),
    receiver(std::move(receiver)) {}

void TraceSimulator::run() {
    while (issuer.issueCommand(*receiver)) continue;
    issuer.drain(*receiver);
}

void TraceSimulator::printStats(std::ostream& statStream) {
    receiver->printStats(statStream);
}