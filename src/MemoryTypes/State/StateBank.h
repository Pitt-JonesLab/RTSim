#pragma once

#include "MemoryTypes/State/Commandable.h"
#include "MemoryTypes/State/State.h"
#include "Stats/StatBlock.h"

namespace NVM::State {

class StateBank : public Commandable {
    public:
    /*
     *   Creates a StateBank in a closed state.
     */
    StateBank();

    /*
     *   Performs a READ command.
     */
    bool read(const Address& address, const RowData& data);

    /*
     *   Performs a WRITE command.
     */
    bool write(const Address& address, const RowData& data);

    /*
     *   Performs an ACTIVATE command.
     */
    bool activate(const Address& address);

    /*
     *   Performs a PRECHARGE command.
     */
    bool precharge(const Address& address);

    /*
     *   Performs a REFRESH command.
     */
    bool refresh();

    void cycle();

    NVM::Stats::ValueStatBlock getStats() const;

    private:
    std::unique_ptr<State> currentState;
    NVM::Stats::ValueStatBlock stats;
};

} // namespace NVM::State