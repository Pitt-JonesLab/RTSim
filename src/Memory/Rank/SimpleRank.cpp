#include "Memory/Rank/SimpleRank.h"

#include "Logging/Logging.h"

#include <functional>

using namespace NVM::Memory;

namespace NVM::Memory {

class RankCommand : public Command {
    public:
    RankCommand() : parent(nullptr), complete(false) {}

    void setParent(Command* p) { parent = p; }

    void notify() {
        if (parent) {
            parent->notify();
        }
        complete = true;
    }

    bool isDone() const { return complete; }

    private:
    Command* parent;
    bool complete;
};

} // namespace NVM::Memory

using namespace NVM::Memory;
using namespace NVM::Simulation;
using namespace NVM::Logging;

using CommandFunc = std::function<Command*()>;

std::unique_ptr<Command> makeRankCommand(CommandFunc& func) {
    auto bankCommand = func();
    if (!bankCommand) return nullptr;

    auto rankCommand = std::unique_ptr<Command>(new RankCommand());
    bankCommand->setParent(rankCommand.get());
    return std::move(rankCommand);
}

SimpleRank::SimpleRank() : totalReads(0), totalWrites(0) {}

Command* SimpleRank::read(uint64_t address, DataBlock data) {
    if (banks.empty()) return nullptr;
    if (currentCommand) return nullptr;

    CommandFunc readFunc = [&]() { return banks[0]->read(address, data); };

    currentCommand = std::move(makeRankCommand(readFunc));
    if (currentCommand) {
        log() << LogLevel::EVENT << "SimpleRank received read\n";
        totalReads++;
    }
    return currentCommand.get();
}

Command* SimpleRank::write(uint64_t address, NVM::Simulation::DataBlock data) {
    if (banks.empty()) return nullptr;
    if (currentCommand) return nullptr;

    CommandFunc writeFunc = [&]() { return banks[0]->write(address, data); };

    currentCommand = std::move(makeRankCommand(writeFunc));
    if (currentCommand) {
        log() << LogLevel::EVENT << "SimpleRank received read\n";
        totalWrites++;
    }
    return currentCommand.get();
}

void SimpleRank::cycle(unsigned int cycles) {
    if (!banks.empty()) banks[0]->cycle(cycles);
    if (!currentCommand) return;
    if (static_cast<RankCommand*>(currentCommand.get())->isDone())
        currentCommand.reset();
}

bool SimpleRank::isEmpty() const { return currentCommand == nullptr; }

void SimpleRank::addBank(std::unique_ptr<Bank> bank) {
    banks.emplace_back(std::move(bank));
}

StatBlock SimpleRank::getStats(std::string tag) const {
    StatBlock stats(tag);

    stats.addStat(&totalReads, "reads");
    stats.addStat(&totalWrites, "writes");

    for (int i = 0; i < banks.size(); i++) {
        stats.addChild(banks[i]->getStats(tag + ".bank" + std::to_string(i)));
    }

    return stats;
}