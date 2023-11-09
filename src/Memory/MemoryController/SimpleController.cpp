#include "Memory/MemoryController/SimpleController.h"

#include "Logging/Logging.h"
#include "Memory/Command/WaitingCommand.h"
#include "Memory/Decoder.h"

#include <functional>

using namespace NVM::Memory;

using namespace NVM::Memory;
using namespace NVM::Simulation;
using namespace NVM::Logging;

SimpleController::SimpleController() :
    totalReads(0),
    totalWrites(0),
    highLevelInstructions(1),
    lowLevelInstructions(1),
    openRow(-1) {}

bool SimpleController::read(uint64_t address, DataBlock data) {
    if (interconnects.empty()) return false;
    if (highLevelInstructions[0].size() == 10) return false;
    if (receivedInst) return false;

    receivedInst = std::make_unique<ReadInstruction>(address, data);
    log() << LogLevel::EVENT << "SimpleController received read\n";
    totalReads++;

    return true;
}

bool SimpleController::write(uint64_t address,
                             NVM::Simulation::DataBlock data) {
    if (interconnects.empty()) return false;
    if (highLevelInstructions[0].size() == 10) return false;
    if (receivedInst) return false;

    receivedInst = std::make_unique<WriteInstruction>(address, data);
    log() << LogLevel::EVENT << "SimpleController received write\n";
    totalWrites++;

    return true;
}

bool SimpleController::rowClone(uint64_t srcAddress, uint64_t destAddress,
                                NVM::Simulation::DataBlock data) {
    return false;
}

bool SimpleController::transverseRead(
    uint64_t baseAddress, uint64_t destAddress,
    std::vector<NVM::Simulation::DataBlock> data) {
    return false;
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
        highLevelInstructions[0].push(std::move(receivedInst));
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
    if (cmd) lowLevelInstructions[0].pop();
}

bool SimpleController::isEmpty() const {
    if (interconnects.empty()) return false;
    return interconnects[0]->isEmpty() && highLevelInstructions[0].empty() &&
           lowLevelInstructions[0].empty() && !receivedInst;
}

StatBlock SimpleController::getStats(std::string tag) const {
    StatBlock stats(tag);

    stats.addStat(&totalReads, "reads");
    stats.addStat(&totalWrites, "writes");

    for (int i = 0; i < interconnects.size(); i++) {
        stats.addChild(
            interconnects[i]->getStats(tag + ".i" + std::to_string(i)));
    }

    return stats;
}

void SimpleController::parseTransaction() {
    if (!lowLevelInstructions[0].empty()) return;
    if (highLevelInstructions[0].empty()) return;

    auto& nextInst = highLevelInstructions[0].front();

    auto nextRow = Decoder::decodeSymbol(Decoder::AddressSymbol::ROW,
                                         nextInst->getAddress());
    if (openRow != nextRow) {
        if (openRow != -1)
            lowLevelInstructions[0].push(
                std::make_unique<PrechargeInstruction>(nextInst->getAddress()));
        lowLevelInstructions[0].push(
            std::make_unique<ActivateInstruction>(nextInst->getAddress()));
        openRow = nextRow;

        log() << LogLevel::EVENT << "Switching to row " << openRow << '\n';
    } else {
        log() << LogLevel::EVENT << "Row buffer hit\n";
    }

    lowLevelInstructions[0].push(std::move(nextInst));
    highLevelInstructions[0].pop();
}