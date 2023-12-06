#include "Memory/MemoryController/SimpleController.h"

#include "Logging/Logging.h"
#include "Memory/Command/WaitingCommand.h"
#include "Modeling/Decoder.h"

#include <algorithm>
#include <functional>
#include <iterator>

using namespace NVM::Memory;

using namespace NVM::Memory;
using namespace NVM::Simulation;
using namespace NVM::Logging;

SimpleController::SimpleController() :
    rowBufferHits(0),
    highLevelInstructions(1),
    lowLevelInstructions(1),
    openRow(-1) {}

bool SimpleController::read(uint64_t address, DataBlock data) {
    if (interconnects.empty()) return false;
    if (highLevelInstructions[0].size() == 20) return false;
    if (receivedInst) return false;

    receivedInst = std::make_unique<ReadInstruction>(address, data);
    log() << LogLevel::EVENT << "SimpleController received read\n";

    return true;
}

bool SimpleController::write(uint64_t address,
                             NVM::Simulation::DataBlock data) {
    if (interconnects.empty()) return false;
    if (highLevelInstructions[0].size() == 20) return false;
    if (receivedInst) return false;

    receivedInst = std::make_unique<WriteInstruction>(address, data);
    log() << LogLevel::EVENT << "SimpleController received write\n";

    return true;
}

bool SimpleController::rowClone(uint64_t srcAddress, uint64_t destAddress,
                                NVM::Simulation::DataBlock data) {
    if (interconnects.empty()) return false;
    if (highLevelInstructions[0].size() == 20) return false;
    if (receivedInst) return false;

    receivedInst =
        std::make_unique<RowCloneInstruction>(srcAddress, destAddress, data);
    log() << LogLevel::EVENT << "SimpleController received RowClone\n";

    return true;
}

bool SimpleController::transverseRead(
    uint64_t baseAddress, uint64_t destAddress,
    std::vector<NVM::Simulation::DataBlock> data) {
    if (interconnects.empty()) return false;
    if (highLevelInstructions[0].size() == 20) return false;
    if (receivedInst) return false;

    receivedInst =
        std::make_unique<PIMInstruction>(baseAddress, destAddress, data);
    log() << LogLevel::EVENT << "SimpleController received RowClone\n";

    return true;
}

bool SimpleController::transverseWrite(
    uint64_t address, std::vector<NVM::Simulation::DataBlock> data) {
    return false;
}

void SimpleController::cycle(unsigned int cycles) {
    if (cycles == 0) return;
    if (!interconnects.empty()) interconnects[0]->cycle(cycles);
    parseTransaction();
    issueFromQueue();
    cycles--;
    if (receivedInst) {
        highLevelInstructions[0].push_back(std::move(receivedInst));
        receivedInst.reset();
    }
    while (cycles && !highLevelInstructions[0].empty()) {
        parseTransaction();
        issueFromQueue();
        cycles--;
    }
}

void SimpleController::issueFromQueue() {
    if (interconnects.empty()) return;
    if (lowLevelInstructions[0].empty()) return;
    auto cmd =
        interconnects[0]->issue(*(lowLevelInstructions[0].front().get()));
    if (cmd) lowLevelInstructions[0].erase(lowLevelInstructions[0].begin());
}

bool SimpleController::isEmpty() const {
    if (interconnects.empty()) return false;
    return interconnects[0]->isEmpty() && highLevelInstructions[0].empty() &&
           lowLevelInstructions[0].empty() && !receivedInst;
}

StatBlock SimpleController::getStats(std::string tag) const {
    StatBlock stats(tag);

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

std::unique_ptr<Instruction> SimpleController::getNextInstruction() {
    if (highLevelInstructions[0].empty()) return nullptr;

    // Try to send row buffer hit
    auto it =
        std::find_if(highLevelInstructions[0].begin(),
                     highLevelInstructions[0].end(), [this](const auto& inst) {
                         auto row = Decoder::decodeSymbol(
                             Decoder::AddressSymbol::ROW, inst->getAddress());
                         return row == translator.getOpenRow();
                     });

    if (it != highLevelInstructions[0].end()) {
        log() << LogLevel::EVENT << "Row buffer hit\n";
        rowBufferHits++;
        auto nextInst = std::move(*it);
        highLevelInstructions[0].erase(it);
        return nextInst;
    }

    // Send next instruction in queue
    auto nextInst = std::move(highLevelInstructions[0].front());
    highLevelInstructions[0].erase(highLevelInstructions[0].begin());
    return std::move(nextInst);
}

void SimpleController::parseTransaction() {
    if (!lowLevelInstructions[0].empty()) return;
    if (highLevelInstructions[0].empty()) return;

    auto nextInst = getNextInstruction();
    if (!nextInst) return;

    auto lowLevelInsts = nextInst->translate(translator);
    lowLevelInstructions[0].reserve(lowLevelInstructions[0].size() +
                                    lowLevelInsts.size());
    lowLevelInstructions[0].insert(
        lowLevelInstructions[0].end(),
        std::make_move_iterator(lowLevelInsts.begin()),
        std::make_move_iterator(lowLevelInsts.end()));
}