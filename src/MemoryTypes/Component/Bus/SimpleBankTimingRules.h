#pragma once

#include "MemoryTypes/Component/Bank/BankCommand.h"
#include "MemoryTypes/Component/TimingRules.h"

#include <map>
#include <unordered_map>

namespace NVM::ComponentType {

class SimpleBankTimingRules : public TimingRules<BankCommand> {
    public:
    SimpleBankTimingRules();

    /*
     *   Signals a command being issued in the current cycle. Allows the
     * implementation to maintain a history of sent commands.
     */
    void issue(BankCommand cmd);

    /*
     *   Returns true if the provided command can be sent this cycle, false
     * otherwise.
     */
    bool validate(BankCommand cmd);

    /*
     *   Signals a cycle event so TimingRules implementations can maintain an
     * accurate history.
     */
    void cycle();

    void updateNextCycle(BankCommand::Opcode opcode, BankCommand cmd);

    private:
    int currentCycle;
    std::unordered_map<BankCommand::Opcode, int> nextCycle;
    std::map<std::pair<BankCommand::Opcode, BankCommand::Opcode>, int> delays;
    BankCommand::Opcode lastCommand;
};

} // namespace NVM::ComponentType