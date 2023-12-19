#include "MemoryTypes/Simple/Bank/SimpleBank.h"

using namespace NVM::Memory;

void SimpleBank::cycle(unsigned int cycles) {
    if (!subArrays.empty()) subArrays[0]->cycle(cycles);
}

bool SimpleBank::isEmpty() const {
    if (subArrays.empty()) return false;
    return subArrays[0]->isEmpty();
}