#include "MemoryTypes/Simple/MemoryController/SimpleController.h"

#include "Logging/Logging.h"
#include "MemoryTypes/Simple/Modeling/Bank/SimpleBank.h"
#include "MemoryTypes/Simple/Modeling/Rank/SimpleRank.h"
#include "MemoryTypes/Simple/Modeling/SubArray/SimpleSubArray.h"

using namespace NVM::Memory;
using namespace NVM::Simulation;
using namespace NVM::Logging;
using namespace NVM::Modeling;

SimpleController::SimpleController() :
    fails(false),
    received(false),
    rInst(NVM::Scheduling::InstructionType::READ, 0) {
    auto sa = std::make_unique<NVM::Modeling::SimpleSubArray>();
    auto bank = std::make_unique<NVM::Modeling::SimpleBank>();
    auto rank = std::make_unique<NVM::Modeling::SimpleRank>();
    bank->addSubArray(std::move(sa));
    rank->addBank(std::move(bank));
    model.addRank(std::move(rank));
}

void SimpleController::failNext() { fails = true; }

bool SimpleController::read(uint64_t address, DataBlock data) {
    if (received || !scheduler.isAvailable()) return false;

    received = true;
    rInst = {NVM::Scheduling::InstructionType::READ, address};
    fails = false;
    log() << LogLevel::EVENT << "SimpleController received read\n";

    return true;
}

bool SimpleController::write(uint64_t address,
                             NVM::Simulation::DataBlock data) {
    if (received || !scheduler.isAvailable()) return false;

    rInst = {NVM::Scheduling::InstructionType::WRITE, address};
    fails = false;
    received = true;
    log() << LogLevel::EVENT << "SimpleController received write\n";

    return true;
}

bool SimpleController::rowClone(uint64_t srcAddress, uint64_t destAddress,
                                NVM::Simulation::DataBlock data) {
    if (received || !scheduler.isAvailable()) return false;

    rInst = {NVM::Scheduling::InstructionType::ROWCLONE, srcAddress};
    fails = false;
    received = true;
    log() << LogLevel::EVENT << "SimpleController received PIM\n"
          << "Address: " << srcAddress << "\n";

    return true;
}

bool SimpleController::transverseRead(
    uint64_t baseAddress, uint64_t destAddress,
    std::vector<NVM::Simulation::DataBlock> data) {
    if (received || !scheduler.isAvailable()) return false;

    rInst = {NVM::Scheduling::InstructionType::PIM, baseAddress};

    // Do instruction twice on failure
    if (fails) {
        scheduler.enqueue(rInst);
        fails = false;
    }

    received = true;
    log() << LogLevel::EVENT << "SimpleController received PIM\n"
          << "Address: " << baseAddress << "\n";

    return true;
}

bool SimpleController::transverseWrite(uint64_t address,
                                       NVM::Simulation::DataBlock data) {
    if (received || !scheduler.isAvailable()) return false;

    rInst = {NVM::Scheduling::InstructionType::TRANSVERSE_WRITE, address};
    received = true;
    log() << LogLevel::EVENT << "SimpleController received TW\n"
          << "Address: " << address << "\n";

    return true;
}

void SimpleController::cycle(unsigned int cycles) {
    if (cycles == 0) return;
    if (!interconnects.empty()) interconnects[0]->cycle(cycles);

    if (received) {
        scheduler.enqueue(rInst);
        rInst = {NVM::Scheduling::InstructionType::READ, 0};
        received = false;
    }
    if (cmdQueue.empty() && !scheduler.isEmpty()) {
        cmdQueue = parser.parseCommands(scheduler.issue(model), model);
        rowBufferHits = scheduler.getRBHits();
    }
    if (!cmdQueue.empty()) {
        if (interconnects[0]->issue(cmdQueue[0])) {
            cmdQueue.erase(cmdQueue.begin());
        }
    }
}

bool SimpleController::isEmpty() const {
    if (interconnects.empty()) return false;
    return scheduler.isEmpty() && cmdQueue.empty() && !received &&
           interconnects[0]->isEmpty();
}

NVM::Stats::StatBlock SimpleController::getStats(std::string tag) const {
    NVM::Stats::StatBlock stats(tag);

    stats.addStat(&rowBufferHits, "row_buffer_hits");

    for (int i = 0; i < interconnects.size(); i++) {
        auto interStats =
            interconnects[i]->getStats(tag + ".i" + std::to_string(i));
        stats.addChildStat(interStats, "activate_energy", "nJ");
        stats.addChildStat(interStats, "read_energy", "nJ");
        stats.addChildStat(interStats, "write_energy", "nJ");
        stats.addChildStat(interStats, "reads");
        stats.addChildStat(interStats, "writes");
        stats.addChildStat(interStats, "activates");
        stats.addChildStat(interStats, "precharges");
        stats.addChildStat(interStats, "row_clones");
        stats.addChildStat(interStats, "transverse_reads");
        stats.addChildStat(interStats, "transverse_writes");
        stats.addChild(interStats);
    }

    return stats;
}