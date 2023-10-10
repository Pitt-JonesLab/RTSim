#include "Memory/SubArray/SimpleSubArray.h"

using namespace NVM::Memory;

bool SimpleSubArray::issue(NVMainRequest* req) { return false; }

bool SimpleSubArray::read(uint64_t address, NVM::Simulation::DataBlock data,
                          unsigned int threadId, unsigned int cycle) {
    return false;
}

bool SimpleSubArray::write(uint64_t address, NVM::Simulation::DataBlock data,
                           unsigned int threadId, unsigned int cycle) {
    return false;
}

void SimpleSubArray::cycle(unsigned int cycles) {}

unsigned int SimpleSubArray::getCurrentCycle() { return 0; }

bool SimpleSubArray::isEmpty() const { return false; }

void SimpleSubArray::printStats(std::ostream& statStream) {}