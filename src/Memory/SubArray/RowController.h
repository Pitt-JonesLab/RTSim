#pragma once

#include "Memory/Command.h"

#include <vector>

namespace NVM::Memory::SubArray {

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

        NVM::Memory::Command* activate(unsigned int row);
        NVM::Memory::Command* precharge(unsigned int row);

    private:
        std::vector<RowState> rowStates;

        unsigned int prechargeTime; // tRP
        unsigned int activateTime; // tRCD
};

}