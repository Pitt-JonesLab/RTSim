#include "MemoryTypes/State/ClosedState.h"

using namespace NVM::State;

NVM::State::ClosedState::ClosedState() : State() {}

void NVM::State::ClosedState::cycle() {}

std::unique_ptr<State> NVM::State::ClosedState::getNext() const {
    return nullptr;
}

bool NVM::State::ClosedState::read(const Address& address,
                                   const RowData& data) {
    return true;
}

bool NVM::State::ClosedState::write(const Address& address,
                                    const RowData& data) {
    return true;
}

bool NVM::State::ClosedState::activate(const Address& address) { return true; }

bool NVM::State::ClosedState::precharge(const Address& address) { return true; }

bool NVM::State::ClosedState::refresh() { return true; }
