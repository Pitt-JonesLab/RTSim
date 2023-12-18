#include "Simulation/TraceSimulator.h"

using namespace NVM::Simulation;

TraceSimulator::TraceSimulator(std::unique_ptr<TraceReader> reader,
                               std::unique_ptr<NVM::Memory::Commandable> rcvr,
                               unsigned int maxCycles) :
    queue(rcvr.get()), receiver(std::move(rcvr)), timer(maxCycles) {
        auto cmd = reader->getNextCommand();
        while (cmd) {
            queue.enqueue(cmd);
            cmd = reader->getNextCommand();
        }
    }

void TraceSimulator::run() {
    while (!queue.isEmpty()) { 
        queue.issue();
        if (timer.cycle(1)) receiver->cycle(1);
        else break; 
    }
    while (!receiver->isEmpty()) {
        if (timer.cycle(1)) receiver->cycle(1);
        else break;
    }
    std::cout << "Exiting at cycle " << timer.getCurrentCycle()
              << " because simCycles " << timer.getMaxCycle() << " reached.\n";
}

void TraceSimulator::printStats(std::ostream& statStream) {
    receiver->printStats(statStream);
}