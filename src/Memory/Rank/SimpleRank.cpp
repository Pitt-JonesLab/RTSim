#include "Memory/Rank/SimpleRank.h"

using namespace NVM::Memory;

bool SimpleRank::issue(NVMainRequest* req) { return false; }

bool SimpleRank::read(uint64_t address, NVM::Simulation::DataBlock data,
                      unsigned int threadId, unsigned int cycle) {
    return false;
}

bool SimpleRank::write(uint64_t address, NVM::Simulation::DataBlock data,
                       unsigned int threadId, unsigned int cycle) {
    return false;
}

void SimpleRank::cycle(unsigned int cycles) {}

unsigned int SimpleRank::getCurrentCycle() { return 0; }

bool SimpleRank::isEmpty() const { return false; }

void SimpleRank::printStats(std::ostream& statStream) {}

void SimpleRank::addBank(std::unique_ptr<Bank> bank) {}