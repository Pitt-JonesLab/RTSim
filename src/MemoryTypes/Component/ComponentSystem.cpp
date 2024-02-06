#include "MemoryTypes/Component/ComponentSystem.h"

#include "Stats/StatBlock.h"

void NVM::ComponentType::ComponentSystem::process() {}

void NVM::ComponentType::ComponentSystem::cycle() {}

NVM::Stats::StatBlock NVM::ComponentType::ComponentSystem::getStats() {
    return Stats::StatBlock();
}

bool NVM::ComponentType::ComponentSystem::read(const Address& address,
                                               const RowData& data) {
    return true;
}

bool NVM::ComponentType::ComponentSystem::write(const Address& address,
                                                const RowData& data) {
    return true;
}

bool NVM::ComponentType::ComponentSystem::rowClone(const Address& srcAddress,
                                                   const Address& destAddress,
                                                   const RowData& data) {
    return true;
}

bool NVM::ComponentType::ComponentSystem::pim(std::vector<Address> operands,
                                              const Address& destAddress,
                                              std::vector<RowData> data) {
    return true;
}

bool NVM::ComponentType::ComponentSystem::isEmpty() const { return true; }

void NVM::ComponentType::ComponentSystem::cycle(unsigned int cycles) {}

void NVM::ComponentType::ComponentSystem::printStats(std::ostream& statStream) {

}
