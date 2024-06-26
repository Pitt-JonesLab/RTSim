#include "MemoryTypes/Component/System/ComponentSystem.h"

#include "MemoryTypes/Component/System/MemoryCommand.h"
#include "Stats/StatBlock.h"

NVM::ComponentType::ComponentSystem::ComponentSystem() {
    bank.setCommandConnection(bus.getCommandConnection());
    bank.setResponseConnection(bus.getResponseConnection());
    controller.setCommandConnection(bus.getCommandConnection());
    controller.setResponseConnection(bus.getResponseConnection());
}

void NVM::ComponentType::ComponentSystem::process() {
    bank.process();
    bus.process();
    controller.process();
}

void NVM::ComponentType::ComponentSystem::cycle() {
    process();

    bank.cycle();
    bus.cycle();
    controller.cycle();
}

NVM::Stats::ValueStatBlock
NVM::ComponentType::ComponentSystem::getStats(std::string tag) {
    Stats::ValueStatBlock systemStats(tag);

    auto controllerStats = controller.getStats("channel0");
    auto busStats = bus.getStats("i0");
    auto bankStats = bank.getStats("rank0.bank0");

    busStats.addChild(bankStats);
    busStats.addChildStat(bankStats, "reads");
    busStats.addChildStat(bankStats, "writes");
    busStats.addChildStat(bankStats, "precharges");
    busStats.addChildStat(bankStats, "activates");
    busStats.addChildStat(bankStats, "row_clones");
    busStats.addChildStat(bankStats, "transverse_reads");

    controllerStats.addChild(busStats);
    controllerStats.addChildStat(busStats, "reads");
    controllerStats.addChildStat(busStats, "writes");
    controllerStats.addChildStat(busStats, "precharges");
    controllerStats.addChildStat(busStats, "activates");
    controllerStats.addChildStat(busStats, "row_clones");
    controllerStats.addChildStat(busStats, "transverse_reads");

    systemStats.addChild(controllerStats);
    systemStats.addChildStat(controllerStats, "reads");
    systemStats.addChildStat(controllerStats, "writes");
    systemStats.addChildStat(controllerStats, "activates");
    systemStats.addChildStat(controllerStats, "precharges");
    systemStats.addChildStat(controllerStats, "row_clones");
    systemStats.addChildStat(controllerStats, "transverse_reads");

    return systemStats;
}

bool NVM::ComponentType::ComponentSystem::read(const Address& address,
                                               const RowData& data) {
    return controller.issue({MemoryCommand::Opcode::READ, address, data});
}

bool NVM::ComponentType::ComponentSystem::write(const Address& address,
                                                const RowData& data) {
    return controller.issue({MemoryCommand::Opcode::WRITE, address, data});
}

bool NVM::ComponentType::ComponentSystem::rowClone(const Address& srcAddress,
                                                   const Address& destAddress,
                                                   const RowData& data) {
    return controller.issue(
        {MemoryCommand::Opcode::ROWCLONE, srcAddress, destAddress, data});
}

// TODO: TR should have a single source address (base row address for TR)
bool NVM::ComponentType::ComponentSystem::pim(std::vector<Address> operands,
                                              const Address& destAddress,
                                              std::vector<RowData> data) {
    return controller.issue({MemoryCommand::Opcode::TRANSVERSE_READ,
                             operands[0], destAddress, data[0]});
}

bool NVM::ComponentType::ComponentSystem::transverseWrite(
    const Address& address, const RowData& data) {
    return true;
}

bool NVM::ComponentType::ComponentSystem::isEmpty() const {
    return !(bank.busy() || bus.busy() || controller.busy());
}

void NVM::ComponentType::ComponentSystem::cycle(unsigned int cycles) {
    for (int i = 0; i < cycles; i++) cycle();
}

void NVM::ComponentType::ComponentSystem::printStats(std::ostream& statStream) {
    auto stats = getStats("system");
    stats.log();
}
