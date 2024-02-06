#pragma once

#include "MemoryTypes/State/Commandable.h"
#include "MemoryTypes/State/StateBank.h"

namespace NVM::State {

class Bus : public Commandable {
    public:
    Bus();

    void setBank(StateBank* b);

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

    bool sendData();

    void cycle();

    void printCycles() const;

    private:
    StateBank* bank;

    enum class BUS_STATE { READ, WRITE, NO_OP };

    BUS_STATE currentState;
    std::vector<BUS_STATE> commandHistory;

    friend std::ostream& operator<<(std::ostream& out, const BUS_STATE state) {
        switch (state) {
            case BUS_STATE::NO_OP:
                out << "no_op";
                break;
            case BUS_STATE::READ:
                out << "read";
                break;
            case BUS_STATE::WRITE:
                out << "write";
                break;
        }
        return out;
    }
};

} // namespace NVM::State