#include "Memory/SubArray/RowController.h"

using namespace NVM::Memory::SubArray;
using NVM::Memory::Command;

RowController::RowController(unsigned int rows, RowTiming timing) {
    rowStates = std::vector<RowState>(rows, RowState::CLOSED);
}

RowState RowController::getState(unsigned int row) const {
    return rowStates[row];
}

void RowController::setState(unsigned int row, RowState state) {
    rowStates[row] = state;
}

RowState& RowController::operator[](unsigned int row) {
    return rowStates[row];
}

const RowState& RowController::operator[](unsigned int row) const {
    return rowStates.at(row);
}

Command* RowController::activate(unsigned int row) {
    return nullptr;
}

Command* RowController::precharge(unsigned int row) {
    return nullptr;
}