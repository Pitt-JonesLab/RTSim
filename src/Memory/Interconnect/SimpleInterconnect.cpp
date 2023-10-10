#include "Memory/Interconnect/SimpleInterconnect.h"

using namespace NVM::Memory;

bool SimpleInterconnect::issue(NVMainRequest* req) { return false; }

bool SimpleInterconnect::read(uint64_t address, NVM::Simulation::DataBlock data,
                              unsigned int threadId, unsigned int cycle) {
    return false;
}

bool SimpleInterconnect::write(uint64_t address,
                               NVM::Simulation::DataBlock data,
                               unsigned int threadId, unsigned int cycle) {
    return false;
}

void SimpleInterconnect::cycle(unsigned int cycles) {}

unsigned int SimpleInterconnect::getCurrentCycle() { return 0; }

bool SimpleInterconnect::isEmpty() const { return false; }

void SimpleInterconnect::printStats(std::ostream& statStream) {}