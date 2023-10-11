#include "Memory/MemorySystem/SimpleSystem.h"

using namespace NVM::Memory;

bool SimpleSystem::issue(NVMainRequest* req) { return false; }

bool SimpleSystem::read(uint64_t address, NVM::Simulation::DataBlock data,
                        unsigned int threadId, unsigned int cycle) {
    return false;
}

bool SimpleSystem::write(uint64_t address, NVM::Simulation::DataBlock data,
                         unsigned int threadId, unsigned int cycle) {
    return false;
}

void SimpleSystem::cycle(unsigned int cycles) {}

unsigned int SimpleSystem::getCurrentCycle() { return 0; }

bool SimpleSystem::isEmpty() const { return false; }

void SimpleSystem::printStats(std::ostream& statStream) {}

void SimpleSystem::addController(std::unique_ptr<MemoryController> controller) {
}