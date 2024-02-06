#include "MemoryTypes/Component/ComponentSystem.h"

#include "Stats/StatBlock.h"

NVM::ComponentType::ComponentSystem::ComponentSystem() {
    bank.setCommandConnection(bus.getCommandConnection());
    bank.setResponseConnection(bus.getResponseConnection());
    controller.setCommandConnection(bus.getCommandConnection());
}

void NVM::ComponentType::ComponentSystem::process() {
    bank.process();
    bus.process();
    controller.process();
}

void NVM::ComponentType::ComponentSystem::cycle() {
    bank.cycle();
    bus.cycle();
    controller.cycle();
}

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

bool NVM::ComponentType::ComponentSystem::isEmpty() const {
    return !(bank.busy() || bus.busy() || controller.busy());
}

void NVM::ComponentType::ComponentSystem::cycle(unsigned int cycles) {
    for (int i = 0; i < cycles; i++) cycle();
}

void NVM::ComponentType::ComponentSystem::printStats(std::ostream& statStream) {

}
