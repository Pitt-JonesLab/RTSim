#include "Simulation/TraceIssuer.h"

#include "Simulation/MemorySystem.h"

#include <iostream>

using namespace NVM::Simulation;

TraceIssuer::TraceIssuer(std::unique_ptr<TraceReader> reader,
                         unsigned int cycles) :
    reader(std::move(reader)),
    timer(cycles) {}

bool TraceIssuer::issue(MemorySystem* memory) {
    auto nextCommand = reader->getNext();
    if (!nextCommand) return false;

    while (!nextCommand->issue(memory)) {
        if (timer.cycle(1)) memory->cycle(1);
        else return false;
    }
    return true;
}

bool TraceIssuer::issueCommand(Commandable& receiver) {
    auto nextCommand = reader->getNextCommand();
    if (!nextCommand) return false;

    while (!nextCommand(receiver)) {
        if (timer.cycle(1)) receiver.cycle(1);
        else return false;
    }
    return true;
}

bool TraceIssuer::drain(MemorySystem* memory) {
    while (!memory->isEmpty()) {
        if (timer.cycle(1)) memory->cycle(1);
        else return false;
    }
    std::cout << "Exiting at cycle " << timer.getCurrentCycle()
              << " because simCycles " << timer.getMaxCycle() << " reached.\n";
    return true;
}

bool TraceIssuer::drain(Commandable& receiver) {
    while (!receiver.isEmpty()) {
        if (timer.cycle(1)) receiver.cycle(1);
        else return false;
    }
    std::cout << "Exiting at cycle " << timer.getCurrentCycle()
              << " because simCycles " << timer.getMaxCycle() << " reached.\n";
    return true;
}