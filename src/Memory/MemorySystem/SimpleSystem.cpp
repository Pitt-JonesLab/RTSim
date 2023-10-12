#include "Memory/MemorySystem/SimpleSystem.h"

#include "Logging/Logging.h"

#include <functional>

using namespace NVM::Memory;

namespace NVM::Memory {

class SystemCommand : public Command {
    public:
    SystemCommand() : parent(nullptr), complete(false) {}

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

bool SimpleSystem::issue(NVMainRequest* req) { return false; }

using CommandFunc = std::function<Command*()>;

std::unique_ptr<Command> makeCommand(CommandFunc& func) {
    auto interconnectCommand = func();
    if (!interconnectCommand) return nullptr;

    auto systemCommand = std::unique_ptr<Command>(new SystemCommand());
    interconnectCommand->setParent(systemCommand.get());
    return std::move(systemCommand);
}

bool SimpleSystem::read(uint64_t address, DataBlock data, unsigned int threadId,
                        unsigned int cycle) {
    if (channels.empty()) return false;
    if (currentCommand) return false;

    CommandFunc readFunc = [&]() { return channels[0]->read(address, data); };

    currentCommand = std::move(makeCommand(readFunc));
    if (currentCommand)
        log() << LogLevel::EVENT << "SimpleSystem received read\n";
    return currentCommand != nullptr;
}

bool SimpleSystem::write(uint64_t address, NVM::Simulation::DataBlock data,
                         unsigned int threadId, unsigned int cycle) {
    if (channels.empty()) return false;
    if (currentCommand) return false;

    CommandFunc writeFunc = [&]() { return channels[0]->write(address, data); };

    currentCommand = std::move(makeCommand(writeFunc));
    if (currentCommand)
        log() << LogLevel::EVENT << "SimpleSystem received write\n";
    return currentCommand != nullptr;
}

void SimpleSystem::cycle(unsigned int cycles) {
    if (!channels.empty()) channels[0]->cycle(cycles);
    if (!currentCommand) return;
    if (static_cast<SystemCommand*>(currentCommand.get())->isDone())
        currentCommand.reset();
}

bool SimpleSystem::isEmpty() const { return currentCommand == nullptr; }

void SimpleSystem::addController(
    std::unique_ptr<MemoryController> interconnect) {
    channels.emplace_back(std::move(interconnect));
}

void SimpleSystem::printStats(std::ostream& statStream) {}

unsigned int SimpleSystem::getCurrentCycle() { return 0; }