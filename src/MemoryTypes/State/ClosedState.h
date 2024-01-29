#pragma once

#include "MemoryTypes/State/State.h"

namespace NVM::State {
class ClosedState : public State {
    public:
    ClosedState();

    /*
     *   Progresses the State forward by 1 cycle.
     *
     *   @return Next State
     */
    std::unique_ptr<State> cycle();

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
};
} // namespace NVM::State