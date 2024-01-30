#pragma once

#include "MemoryTypes/State/Commandable.h"
#include "MemoryTypes/State/State.h"

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

    private:
    std::unique_ptr<State> currentState;
};

} // namespace NVM::State