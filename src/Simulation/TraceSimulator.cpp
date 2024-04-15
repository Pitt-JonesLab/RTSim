#include "Simulation/TraceSimulator.h"

#include "Utils/ConfigParser.h"

using namespace NVM::Simulation;
using namespace NVM::Memory;

TraceSimulator::TraceSimulator(std::unique_ptr<TraceReader> reader,
                               std::unique_ptr<NVM::Memory::MemorySystem> rcvr,
                               unsigned int maxCycles) :
    queue(rcvr.get()),
    receiver(std::move(rcvr)),
    timer(maxCycles) {

    ConfigParser::registerValue<bool>("DebugMode", false, &debugMode);

    auto cmd = reader->getNextCommand();
    while (cmd) {
        queue.enqueue(cmd);
        cmd = reader->getNextCommand();
    }
}

void TraceSimulator::run() {
    while (!queue.isEmpty()) {
        if (debugMode) {
            std::cout << "Cycle " << timer.getCurrentCycle() << ":\n";
            std::cin.get();
        }
        queue.issue();
        if (timer.cycle(1)) receiver->cycle(1);
        else break;
    }
    while (!receiver->isEmpty()) {
        if (debugMode) {
            std::cout << "Cycle " << timer.getCurrentCycle() << ":\n";
            std::cin.get();
        }
        if (timer.cycle(1)) receiver->cycle(1);
        else break;
    }
    std::cout << std::dec << "Exiting at cycle " << timer.getCurrentCycle()
              << " because simCycles " << timer.getMaxCycle() << " reached.\n";
}

void TraceSimulator::printStats(std::ostream& statStream) {
    receiver->printStats(statStream);
}