#include "Memory/MemoryController/SimpleController.h"

#include <functional>

using namespace NVM::Memory;

namespace NVM::Memory {

class ControllerCommand : public Command {
    public:
    ControllerCommand() : parent(nullptr), complete(false) {}

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
    auto interconnectCommand = func();
    if (!interconnectCommand) return nullptr;

    auto systemCommand = std::unique_ptr<Command>(new ControllerCommand());
    interconnectCommand->setParent(systemCommand.get());
    return std::move(systemCommand);
}

Command* SimpleController::read(uint64_t address, DataBlock data) {
    if (interconnects.empty()) return nullptr;
    if (currentCommand) return nullptr;

    CommandFunc readFunc = [&]() {
        return interconnects[0]->read(address, data);
    };

    currentCommand = std::move(makeCommand(readFunc));
    return currentCommand.get();
}

Command* SimpleController::write(uint64_t address,
                                 NVM::Simulation::DataBlock data) {
    if (interconnects.empty()) return nullptr;
    if (currentCommand) return nullptr;

    CommandFunc writeFunc = [&]() {
        return interconnects[0]->write(address, data);
    };

    currentCommand = std::move(makeCommand(writeFunc));
    return currentCommand.get();
}

void SimpleController::cycle(unsigned int cycles) {
    if (!interconnects.empty()) interconnects[0]->cycle(cycles);
    if (!currentCommand) return;
    if (static_cast<ControllerCommand*>(currentCommand.get())->isDone())
        currentCommand.reset();
}

bool SimpleController::isEmpty() const { return currentCommand == nullptr; }

void SimpleController::addInterconnect(
    std::unique_ptr<Interconnect> interconnect) {
    interconnects.emplace_back(std::move(interconnect));
}