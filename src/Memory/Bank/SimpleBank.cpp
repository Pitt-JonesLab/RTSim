#include "Memory/Bank/SimpleBank.h"

#include "Logging/Logging.h"

#include <functional>

namespace NVM::Memory {

class BankCommand : public Command {
    public:
    BankCommand() : parent(nullptr), complete(false) {}

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

std::unique_ptr<Command> makeCommand(CommandFunc& func) {
    auto subArrayCommand = func();
    if (!subArrayCommand) return nullptr;

    auto bankCommand = std::unique_ptr<Command>(new BankCommand());
    subArrayCommand->setParent(bankCommand.get());
    return std::move(bankCommand);
}

Command* SimpleBank::read(uint64_t address, DataBlock data) {
    if (subArrays.empty()) return nullptr;
    if (currentCommand) return nullptr;

    CommandFunc readFunc = [&]() { return subArrays[0]->read(address, data); };

    currentCommand = std::move(makeCommand(readFunc));
    if (currentCommand)
        log() << LogLevel::EVENT << "SimpleBank received read\n";
    return currentCommand.get();
}

Command* SimpleBank::write(uint64_t address, NVM::Simulation::DataBlock data) {
    if (subArrays.empty()) return nullptr;
    if (currentCommand) return nullptr;

    CommandFunc writeFunc = [&]() {
        return subArrays[0]->write(address, data);
    };

    currentCommand = std::move(makeCommand(writeFunc));
    if (currentCommand)
        log() << LogLevel::EVENT << "SimpleBank received write\n";
    return currentCommand.get();
}

void SimpleBank::cycle(unsigned int cycles) {
    if (!subArrays.empty()) subArrays[0]->cycle(cycles);
    if (!currentCommand) return;
    if (static_cast<BankCommand*>(currentCommand.get())->isDone())
        currentCommand.reset();
}

bool SimpleBank::isEmpty() const { return currentCommand == nullptr; }

void SimpleBank::addSubArray(std::unique_ptr<SubArray> subArray) {
    subArrays.emplace_back(std::move(subArray));
}

StatBlock SimpleBank::getStats(std::string tag) const {
    StatBlock stats(tag);

    for (int i = 0; i < subArrays.size(); i++) {
        stats.addChild(
            subArrays[i]->getStats(tag + ".subArray" + std::to_string(i)));
    }

    return stats;
}