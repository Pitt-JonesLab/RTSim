#include "Memory/MemoryController/SimpleController.h"

using namespace NVM::Memory;

bool SimpleController::issue(NVMainRequest* req) { return false; }

bool SimpleController::read(uint64_t address, NVM::Simulation::DataBlock data,
                            unsigned int threadId, unsigned int cycle) {
    return false;
}

bool SimpleController::write(uint64_t address, NVM::Simulation::DataBlock data,
                             unsigned int threadId, unsigned int cycle) {
    return false;
}

void SimpleController::cycle(unsigned int cycles) {}

unsigned int SimpleController::getCurrentCycle() { return 0; }

bool SimpleController::isEmpty() const { return false; }

void SimpleController::printStats(std::ostream& statStream) {}

void SimpleController::addInterconnect(
    std::unique_ptr<Interconnect> interconnect) {}