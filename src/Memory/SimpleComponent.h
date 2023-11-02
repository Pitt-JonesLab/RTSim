#pragma once

#include "Memory/Component.h"

namespace NVM::Memory {

class CommandContainer {
    public:
        virtual bool available() const = 0;
        virtual bool addCommand(std::unique_ptr<Command> cmd) = 0;
        virtual void removeFinished();
};

class SimpleComponent : public Component {
    public:
    template <typename Func, typename... Args>
    Command* handleCommand(Args... args) { 
        return (children[0]->*Func)(args...);

        if (children.empty()) return false;

        auto channelCmd = (children[0]->*Func)(args...);
        if (!channelCmd) return false;
        //totalWrites++;
        //log() << LogLevel::EVENT << "SimpleSystem received write\n";
        commands.addCommand(std::make_unique<WaitingCommand>(channelCmd));
        return true; 
    }

    Command* read(uint64_t address, NVM::Simulation::DataBlock data);
    Command* write(uint64_t address, NVM::Simulation::DataBlock data);

    Command* rowClone(uint64_t srcAddress, uint64_t destAddress,
                      NVM::Simulation::DataBlock data);

    Command* transverseRead(uint64_t baseAddress, uint64_t destAddress,
                            std::vector<NVM::Simulation::DataBlock> data);

    Command* transverseWrite(uint64_t address,
                             std::vector<NVM::Simulation::DataBlock> data);

    bool isEmpty() const;

    void cycle(unsigned int cycles);

    StatBlock getStats(std::string tag) const;

    private:
    std::vector<std::unique_ptr<SimpleComponent>> children;
    std::unique_ptr<CommandContainer> commands;
};

} // namespace NVM::Memory