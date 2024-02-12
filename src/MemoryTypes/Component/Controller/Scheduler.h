#pragma once

#include "MemoryTypes/Component/Controller/BankModel.h"
#include "MemoryTypes/Component/MemoryCommand.h"

#include <vector>

namespace NVM::ComponentType {

class Scheduler {
    public:
    Scheduler();

    MemoryCommand getNext(const BankModel& model);

    void issue(MemoryCommand cmd);

    size_t getSize() const;
    int getRowBufferHits() const;

    private:
    std::vector<MemoryCommand> cmdQueue;

    int rowBufferHits;
};

} // namespace NVM::ComponentType