#include "Memory/Interconnect/SimpleInterconnect.h"

using namespace NVM::Memory;

void SimpleInterconnect::cycle(unsigned int cycles) {
    if (!ranks.empty()) ranks[0]->cycle(cycles);
}

bool SimpleInterconnect::isEmpty() const { 
    if (ranks.empty()) return false;
    return ranks[0]->isEmpty();
}