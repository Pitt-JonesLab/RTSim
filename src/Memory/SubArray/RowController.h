#pragma once

#include "Memory/TimedCommand.h"

#include <vector>
#include <memory>

namespace NVM::Memory {

enum class RowState {
    OPEN,
    CLOSED
};

struct RowTiming {
    unsigned int prechargeTime;
    unsigned int activateTime;
};

/*
*   Assigns each row in a SubArray to a state.
*   Also handles ACTIVATE and PRECHARGE
*/
class RowController {
    public:
        RowController(unsigned int rows, RowTiming timing);

        RowState getState(unsigned int row) const;
        void setState(unsigned int row, RowState state);

        RowState& operator[](unsigned int row);
        const RowState& operator[](unsigned int row) const;

        Command* activate(unsigned int row);
        Command* precharge(unsigned int row);

        void cycle(unsigned int cycles);

        Command* closeRow();
        bool rowIsOpen(unsigned int row);

    private:
        std::vector<RowState> rowStates;

        unsigned int prechargeTime; // tRP
        unsigned int activateTime; // tRCD
        unsigned int openRow;

        std::unique_ptr<TimedCommand> currentCmd;
};

}