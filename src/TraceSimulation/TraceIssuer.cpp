#include "src/TraceSimulation/TraceIssuer.h"

#include "include/NVMainRequest.h"
#include "src/TraceSimulation/MemorySystem.h"

#include <iostream>

using namespace NVM::Simulation;

TraceIssuer::TraceIssuer(std::unique_ptr<TraceReader> reader,
                         unsigned int cycles) :
    reqMaker(std::move(reader)),
    timer(cycles) {}

bool TraceIssuer::issue(MemorySystem* memory) {
    auto nextReq = reqMaker.getRequest();
    if (!nextReq) return false;

    int waitCycles = nextReq->arrivalCycle - timer.getCurrentCycle();
    waitCycles = (waitCycles > 0) ? waitCycles : 0;
    auto realCycles = timer.cycle(waitCycles);
    memory->cycle(realCycles);
    if (realCycles < waitCycles) return false;

    while (!memory->issue(nextReq)) {
        if (timer.cycle(1)) memory->cycle(1);
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