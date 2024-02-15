#include "MemoryTypes/Component/Bank/Bank.h"

#include "Logging/Logging.h"
#include "MemoryTypes/Component/Bank/BankCommand.h"
#include "MemoryTypes/Component/Bank/ClosedState.h"
#include "Stats/StatBlock.h"
#include "Utils/ConfigParser.h"

using namespace NVM::ComponentType;
using namespace NVM::Memory;

NVM::ComponentType::Bank::Bank() {
    info.responseConnection = nullptr;
    info.commandConnection = nullptr;

    ConfigParser::registerValue("ReadEnergy", 1.0,
                                &info.energyCosts.readEnergy);
    ConfigParser::registerValue("WriteEnergy", 1.0,
                                &info.energyCosts.writeEnergy);
    ConfigParser::registerValue("ActivateEnergy", 1.0,
                                &info.energyCosts.activateEnergy);
    ConfigParser::registerValue("CopyEnergy", 1.0,
                                &info.energyCosts.copyEnergy);
    ConfigParser::registerValue("TREnergy", 1.0,
                                &info.energyCosts.transverseReadEnergy);

    stats.addStat(0, "reads");
    stats.addStat<double>(0.0, "read_energy", "nJ");
    stats.addStat(0, "writes");
    stats.addStat<double>(0.0, "write_energy", "nJ");
    stats.addStat(0, "precharges");
    stats.addStat(0, "activates");
    stats.addStat<double>(0.0, "activate_energy", "nJ");
    stats.addStat(0, "row_clones");
    stats.addStat<double>(0.0, "copy_energy", "nJ");
    stats.addStat(0, "transverse_reads");
    stats.addStat<double>(0.0, "transverse_read_energy", "nJ");
    stats.addStat<double>(0.0, "total_energy", "nJ");
    stats.addStat(0, "pim_faults");
    stats.addStat(0, "uncorrectable_pim_faults");

    initialize(std::make_unique<ClosedState>(info));
}

void NVM::ComponentType::Bank::setResponseConnection(
    Connection<BankResponse>* connection) {
    info.responseConnection = connection;
    state = std::make_unique<ClosedState>(info);
}

void NVM::ComponentType::Bank::setCommandConnection(
    Connection<BankCommand>* connection) {
    info.commandConnection = connection;
    state = std::make_unique<ClosedState>(info);
}

bool NVM::ComponentType::Bank::busy() const { return state->busy(); }
