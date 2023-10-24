#include "Memory/SubArray/RowController.h"

#include "Memory/TimedCommand.h"

using namespace NVM::Memory::SubArray;
using NVM::Memory::Command;

RowController::RowController(unsigned int rows, RowTiming timing) :
    currentCmd(nullptr), 
    rowStates(rows, RowState::CLOSED),
    prechargeTime(timing.prechargeTime), 
    activateTime(timing.activateTime) { }

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
    currentCmd = std::unique_ptr<Command>(new TimingCommand(activateTime));
    return currentCmd.get();
}

Command* RowController::precharge(unsigned int row) {
    currentCmd = std::unique_ptr<Command>(new TimingCommand(prechargeTime));
    return currentCmd.get();
}

void RowController::cycle(unsigned int cycles) {
    if (currentCmd) currentCmd->cycle(cycles);
}