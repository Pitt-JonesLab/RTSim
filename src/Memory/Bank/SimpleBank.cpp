#include "Memory/Bank/SimpleBank.h"

using namespace NVM::Memory;

Command* SimpleBank::read(uint64_t address, NVM::Simulation::DataBlock data) {
    return nullptr;
}

Command* SimpleBank::write(uint64_t address, NVM::Simulation::DataBlock data) {
    return nullptr;
}

void SimpleBank::cycle(unsigned int cycles) {}

bool SimpleBank::isEmpty() const { return false; }

void SimpleBank::addSubArray(std::unique_ptr<SubArray> subArray) {}