#include "Memory/MemoryController/SimpleController.h"

#include "Logging/Logging.h"
#include "Memory/Command/WaitingCommand.h"

#include <functional>

using namespace NVM::Memory;

using namespace NVM::Memory;
using namespace NVM::Simulation;
using namespace NVM::Logging;

SimpleController::SimpleController() :
    totalReads(0),
    totalWrites(0),
    commandQueues(1) {}

bool SimpleController::read(uint64_t address, DataBlock data) {
    if (interconnects.empty()) return false;
    if (commandQueues[0].size() == 10) return false;
    if (receivedInst) return false;

    receivedInst = std::make_unique<ReadInstruction>(address, data);
    log() << LogLevel::EVENT << "SimpleController received read\n";
    totalReads++;

    return true;
}

bool SimpleController::write(uint64_t address,
                             NVM::Simulation::DataBlock data) {
    if (interconnects.empty()) return false;
    if (commandQueues[0].size() == 10) return false;
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
    if (!commandQueues[0].empty()) issueFromQueue();
    cycles--;
    if (receivedInst) {
        commandQueues[0].push(std::move(receivedInst));
        receivedInst.reset();
    }
    while (cycles && !commandQueues[0].empty()) {
        issueFromQueue();
        cycles--;
    }
}

void SimpleController::issueFromQueue() {
    if (interconnects.empty()) return;
    auto cmd = interconnects[0]->issue(*(commandQueues[0].front().get()));
    if (cmd) commandQueues.front().pop();
}

bool SimpleController::isEmpty() const {
    if (interconnects.empty()) return false;
    return interconnects[0]->isEmpty() && commandQueues[0].empty() &&
           !receivedInst;
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