#include "Memory/MemorySystem/SimpleSystem.h"

#include "Logging/Logging.h"
#include "Modeling/Decoder.h"

using namespace NVM::Memory;
using namespace NVM::Simulation;
using namespace NVM::Logging;

SimpleSystem::SimpleSystem() :
    totalReads(0),
    totalWrites(0),
    totalRowClones(0),
    totalPIMs(0),
    currentCycle(0) {}

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
    NVM::Stats::StatBlock stats("system");

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

void SimpleSystem::failNext() { channels[0]->failNext(); }

bool SimpleSystem::read(const Address& address, const RowData& data) {
    if (!available()) return false;

    auto channelCmd = channels[0]->read(address.getData(), data);
    if (!channelCmd) return false;
    totalReads++;
    log() << LogLevel::EVENT << "SimpleSystem received read at row "
          << NVM::Modeling::decodeSymbol(NVM::Modeling::AddressSymbol::ROW,
                                         address.getData())
          << "\n";
    return true;
}

bool SimpleSystem::write(const Address& address, const RowData& data) {
    if (!available()) return false;

    auto channelCmd = channels[0]->write(address.getData(), data);
    if (!channelCmd) return false;
    totalWrites++;
    log() << LogLevel::EVENT << "SimpleSystem received write\n";
    return true;
}

bool SimpleSystem::rowClone(const Address& srcAddress,
                            const Address& destAddress, const RowData& data) {
    if (!available()) return false;

    auto channelCmd = channels[0]->rowClone(srcAddress.getData(),
                                            destAddress.getData(), data);
    if (!channelCmd) return false;
    totalRowClones++;
    log() << LogLevel::EVENT << "SimpleSystem received RowClone\n";
    return true;
}

bool SimpleSystem::refresh(const Address& bankAddress) { return false; }

bool SimpleSystem::pim(std::vector<Address> operands,
                       const Address& destAddress, std::vector<RowData> data) {
    if (!available()) return false;

    auto channelCmd = channels[0]->transverseRead(
        operands[0].getData(), destAddress.getData(), {data[0]});
    if (!channelCmd) return false;
    totalPIMs++;
    log() << LogLevel::EVENT << "SimpleSystem received PIM\n";
    return true;
}