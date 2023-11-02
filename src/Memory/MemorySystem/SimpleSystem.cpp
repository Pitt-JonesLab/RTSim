#include "Memory/MemorySystem/SimpleSystem.h"

#include "Logging/Logging.h"
#include "Memory/Command/WaitingCommand.h"

#include <functional>

using namespace NVM::Memory;

using namespace NVM::Memory;
using namespace NVM::Simulation;
using namespace NVM::Logging;

bool SimpleSystem::issue(NVMainRequest* req) { return false; }

using CommandFunc = std::function<Command*()>;

SimpleSystem::SimpleSystem() : totalReads(0), totalWrites(0), currentCycle(0) {}

bool SimpleSystem::read(uint64_t address, DataBlock data, unsigned int threadId,
                        unsigned int cycle) {
    if (!available()) return false;

    auto channelCmd = channels[0]->read(address, data);
    if (!channelCmd) return false;
    totalReads++;
    log() << LogLevel::EVENT << "SimpleSystem received read\n";
    runningCommands.push_back(std::make_unique<WaitingCommand>(channelCmd));
    return true;
}

bool SimpleSystem::write(uint64_t address, NVM::Simulation::DataBlock data,
                         unsigned int threadId, unsigned int cycle) {
    if (!available()) return false;

    auto channelCmd = channels[0]->write(address, data);
    if (!channelCmd) return false;
    totalWrites++;
    log() << LogLevel::EVENT << "SimpleSystem received write\n";
    runningCommands.push_back(std::make_unique<WaitingCommand>(channelCmd));
    return true;
}

bool SimpleSystem::rowClone(uint64_t srcAddress, uint64_t destAddress,
                            NVM::Simulation::DataBlock data,
                            unsigned int threadId, unsigned int cycle) {
    return true;
}

bool SimpleSystem::transverseRead(
    uint64_t baseAddress, uint64_t destAddress,
    std::vector<NVM::Simulation::DataBlock> inputRows, unsigned int threadId,
    unsigned int cycle) {
    return true;
}

bool SimpleSystem::transverseWrite(
    uint64_t baseAddress, std::vector<NVM::Simulation::DataBlock> writeData,
    unsigned int threadId, unsigned int cycle) {
    return true;
}

bool SimpleSystem::shift(uint64_t address, unsigned int shiftAmount,
                         NVM::Simulation::DataBlock data, unsigned int threadId,
                         unsigned int cycle) {
    return true;
}

bool SimpleSystem::available() const { return !channels.empty(); }

void SimpleSystem::cycle(unsigned int cycles) {
    if (!channels.empty()) channels[0]->cycle(cycles);
    currentCycle += cycles;
    auto it = std::remove_if(runningCommands.begin(), runningCommands.end(),
                   [](const std::unique_ptr<WaitingCommand>& cmd) {
                       return cmd->isDone();
                   });
    runningCommands.erase(it, runningCommands.end());
}

bool SimpleSystem::isEmpty() const { return runningCommands.empty(); }

void SimpleSystem::addController(
    std::unique_ptr<MemoryController> interconnect) {
    channels.emplace_back(std::move(interconnect));
}

void SimpleSystem::printStats(std::ostream& statStream) {
    StatBlock stats("system");

    stats.addStat(&totalReads, "reads");
    stats.addStat(&totalWrites, "writes");

    for (int i = 0; i < channels.size(); i++) {
        stats.addChild(
            channels[i]->getStats("system.channel" + std::to_string(i)));
    }

    stats.log(statStream);
}

unsigned int SimpleSystem::getCurrentCycle() { return currentCycle; }