#include "MemoryTypes/State/StateSystem.h"

using namespace NVM::State;
using NVM::Address;
using NVM::RowData;

NVM::State::StateSystem::StateSystem() { bus.setBank(&bank); }

bool StateSystem::read(const Address& address, const RowData& data) {
    return bus.read(address, data);
}

bool StateSystem::write(const Address& address, const RowData& data) {
    return bus.write(address, data);
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

void StateSystem::cycle(unsigned int cycles) {
    for (int i = 0; i < cycles; i++) bus.cycle();
}

void StateSystem::printStats(std::ostream& statStream) {
    auto stats = bank.getStats();
    stats.log();
    bus.printCycles();
}
