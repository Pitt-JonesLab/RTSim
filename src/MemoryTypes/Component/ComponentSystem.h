#pragma once

#include "Memory/MemorySystem.h"
#include "MemoryTypes/Component/Bank/Bank.h"
#include "MemoryTypes/Component/Bus.h"
#include "MemoryTypes/Component/Component.h"
#include "MemoryTypes/Component/Controller/MemoryController.h"

namespace NVM::ComponentType {

class ComponentSystem : public NVM::Memory::MemorySystem, Component {
    public:
    ComponentSystem();

    /*
     *  Processes incoming commands from upstream Components
     */
    void process();

    /*
     *  Sends outgoing commands to downstream Components
     */
    void cycle();

    /*
     *  Returns all stat values associated with this Component
     */
    Stats::ValueStatBlock getStats(std::string tag);

    bool read(const Address& address, const RowData& data);

    bool write(const Address& address, const RowData& data);

    bool rowClone(const Address& srcAddress, const Address& destAddress,
                  const RowData& data);

    bool pim(std::vector<Address> operands, const Address& destAddress,
             std::vector<RowData> data);

    bool isEmpty() const;

    void cycle(unsigned int cycles);

    void printStats(std::ostream& statStream);

    private:
    Bank bank;
    MemoryController controller;
    Bus bus;
};

} // namespace NVM::ComponentType