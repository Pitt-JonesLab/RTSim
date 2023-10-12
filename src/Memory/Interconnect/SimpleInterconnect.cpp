#include "Memory/Interconnect/SimpleInterconnect.h"

using namespace NVM::Memory;

Command* SimpleInterconnect::read(uint64_t address,
                                  NVM::Simulation::DataBlock data) {
    return nullptr;
}

Command* SimpleInterconnect::write(uint64_t address,
                                   NVM::Simulation::DataBlock data) {
    return nullptr;
}

void SimpleInterconnect::cycle(unsigned int cycles) {}

bool SimpleInterconnect::isEmpty() const { return false; }

void SimpleInterconnect::addRank(std::unique_ptr<Rank> rank) {}