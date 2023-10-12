#include "Memory/Interconnect/SimpleInterconnect.h"

#include <functional>

using namespace NVM::Memory;

namespace NVM::Memory {

class InterconnectCommand : public Command {
    public:
    InterconnectCommand() : parent(nullptr), complete(false) {}

    void setParent(Command* p) { parent = p; }

    void notify() {
        if (parent) {
            parent->notify();
            complete = true;
        }
    }

    bool isDone() const { return complete; }

    private:
    Command* parent;
    bool complete;
};

} // namespace NVM::Memory

using namespace NVM::Memory;
using namespace NVM::Simulation;

using CommandFunc = std::function<Command*()>;

std::unique_ptr<Command> makeCommand(CommandFunc& func) {
    auto rankCommand = func();
    if (!rankCommand) return nullptr;

    auto interCommand = std::unique_ptr<Command>(new InterconnectCommand());
    rankCommand->setParent(interCommand.get());
    return std::move(interCommand);
}

Command* SimpleInterconnect::read(uint64_t address, DataBlock data) {
    if (ranks.empty()) return nullptr;
    if (currentCommand) return nullptr;

    CommandFunc readFunc = [&]() { return ranks[0]->read(address, data); };

    currentCommand = std::move(makeCommand(readFunc));
    return currentCommand.get();
}

Command* SimpleInterconnect::write(uint64_t address,
                                   NVM::Simulation::DataBlock data) {
    if (ranks.empty()) return nullptr;
    if (currentCommand) return nullptr;

    CommandFunc writeFunc = [&]() { return ranks[0]->write(address, data); };

    currentCommand = std::move(makeCommand(writeFunc));
    return currentCommand.get();
}

void SimpleInterconnect::cycle(unsigned int cycles) {
    if (!ranks.empty()) ranks[0]->cycle(cycles);
    if (!currentCommand) return;
    if (static_cast<InterconnectCommand*>(currentCommand.get())->isDone())
        currentCommand.reset();
}

bool SimpleInterconnect::isEmpty() const { return currentCommand == nullptr; }

void SimpleInterconnect::addRank(std::unique_ptr<Rank> rank) {
    ranks.emplace_back(std::move(rank));
}