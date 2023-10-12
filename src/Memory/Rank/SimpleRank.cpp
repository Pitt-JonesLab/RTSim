#include "Memory/Rank/SimpleRank.h"

using namespace NVM::Memory;

Command* SimpleRank::read(uint64_t address, NVM::Simulation::DataBlock data) {
    return nullptr;
}

Command* SimpleRank::write(uint64_t address, NVM::Simulation::DataBlock data) {
    return nullptr;
}

bool SimpleRank::isEmpty() const { return false; }

void SimpleRank::cycle(unsigned int cycles) {}

void SimpleRank::addBank(std::unique_ptr<Bank> bank) {}