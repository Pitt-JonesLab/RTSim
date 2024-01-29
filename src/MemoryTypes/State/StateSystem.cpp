#include "MemoryTypes/State/StateSystem.h"

using namespace NVM::State;
using NVM::Address;
using NVM::RowData;

bool StateSystem::read(const Address& address, const RowData& data) {
    return true;
}

bool StateSystem::write(const Address& address, const RowData& data) {
    return true;
}

bool StateSystem::rowClone(const Address& srcAddress,
                           const Address& destAddress, const RowData& data) {
    return true;
}

bool StateSystem::pim(std::vector<Address> operands, const Address& destAddress,
                      std::vector<RowData> data) {
    return true;
}

bool StateSystem::isEmpty() const { return true; }

void StateSystem::cycle(unsigned int cycles) {}

void StateSystem::printStats(std::ostream& statStream) {}