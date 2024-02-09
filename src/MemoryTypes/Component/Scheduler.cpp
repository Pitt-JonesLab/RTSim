#include "MemoryTypes/Component/Scheduler.h"

#include "MemoryTypes/Simple/Modeling/Decoder.h"

#include <algorithm>

using namespace NVM::ComponentType;
using namespace NVM::Modeling;

NVM::ComponentType::Scheduler::Scheduler() : rowBufferHits(0) {}

MemoryCommand NVM::ComponentType::Scheduler::getNext(const BankModel& bank) {
    auto rbHit = std::find_if(
        cmdQueue.begin(), cmdQueue.end(), [&bank](MemoryCommand& cmd) {
            auto cmdRow =
                decodeSymbol(AddressSymbol::ROW, cmd.getAddress().getData());
            return cmdRow == bank.getOpenRow();
        });

    if (rbHit != cmdQueue.end()) {
        rowBufferHits++;
        auto cmd = *rbHit;
        cmdQueue.erase(rbHit);
        return cmd;
    }

    auto cmd = cmdQueue.front();
    cmdQueue.erase(cmdQueue.begin());
    return cmd;
}

void NVM::ComponentType::Scheduler::issue(MemoryCommand cmd) {
    cmdQueue.push_back(cmd);
}

size_t NVM::ComponentType::Scheduler::getSize() const {
    return cmdQueue.size();
}

int NVM::ComponentType::Scheduler::getRowBufferHits() const {
    return rowBufferHits;
}
