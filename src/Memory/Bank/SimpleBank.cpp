#include "Memory/Bank/SimpleBank.h"

using namespace NVM::Memory;

bool SimpleBank::issue(NVMainRequest* req) { return false; }

bool SimpleBank::read(uint64_t address, NVM::Simulation::DataBlock data,
                      unsigned int threadId, unsigned int cycle) {
    return false;
}

bool SimpleBank::write(uint64_t address, NVM::Simulation::DataBlock data,
                       unsigned int threadId, unsigned int cycle) {
    return false;
}

void SimpleBank::cycle(unsigned int cycles) {}

unsigned int SimpleBank::getCurrentCycle() { return 0; }

bool SimpleBank::isEmpty() const { return false; }

void SimpleBank::printStats(std::ostream& statStream) {}