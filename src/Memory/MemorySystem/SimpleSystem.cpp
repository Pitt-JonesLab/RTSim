#include "Memory/MemorySystem/SimpleSystem.h"

#include "Logging/Logging.h"
#include "Memory/Command/WaitingCommand.h"
#include "Memory/Decoder.h"

#include <functional>

using namespace NVM::Memory;
using namespace NVM::Simulation;
using namespace NVM::Logging;

bool SimpleSystem::issue(NVMainRequest* req) { return false; }

using CommandFunc = std::function<Command*()>;

SimpleSystem::SimpleSystem() :
    totalReads(0),
    totalWrites(0),
    totalRowClones(0),
    currentCycle(0) {}

bool SimpleSystem::read(uint64_t address, DataBlock data, unsigned int threadId,
                        unsigned int cycle) {
    if (!available()) return false;

    auto channelCmd = channels[0]->read(address, data);
    if (!channelCmd) return false;
    totalReads++;
    log() << LogLevel::EVENT << "SimpleSystem received read at row "
          << Decoder::decodeSymbol(Decoder::AddressSymbol::ROW, address)
          << "\n";
    return true;
}

bool SimpleSystem::write(uint64_t address, NVM::Simulation::DataBlock data,
                         unsigned int threadId, unsigned int cycle) {
    if (!available()) return false;

    auto channelCmd = channels[0]->write(address, data);
    if (!channelCmd) return false;
    totalWrites++;
    log() << LogLevel::EVENT << "SimpleSystem received write\n";
    return true;
}

bool SimpleSystem::rowClone(uint64_t srcAddress, uint64_t destAddress,
                            NVM::Simulation::DataBlock data,
                            unsigned int threadId, unsigned int cycle) {
    if (!available()) return false;

    auto channelCmd = channels[0]->rowClone(srcAddress, destAddress, data);
    if (!channelCmd) return false;
    totalRowClones++;
    log() << LogLevel::EVENT << "SimpleSystem received RowClone\n";
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
}

bool SimpleSystem::isEmpty() const {
    if (channels.empty()) return false;
    return channels[0]->isEmpty();
}

void SimpleSystem::addController(std::unique_ptr<MemoryController> controller) {
    channels.emplace_back(std::move(controller));
}

void SimpleSystem::printStats(std::ostream& statStream) {
    StatBlock stats("system");

    stats.addStat(&totalReads, "reads");
    stats.addStat(&totalWrites, "writes");
    stats.addStat(&totalRowClones, "row_clones");

    for (int i = 0; i < channels.size(); i++) {
        stats.addChild(
            channels[i]->getStats("system.channel" + std::to_string(i)));
    }

    stats.log(statStream);
}

unsigned int SimpleSystem::getCurrentCycle() { return currentCycle; }