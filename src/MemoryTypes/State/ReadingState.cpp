#include "MemoryTypes/State/ReadingState.h"

using namespace NVM::State;

NVM::State::ReadingState::ReadingState() : State() {}

std::unique_ptr<State> NVM::State::ReadingState::cycle() {
    return std::make_unique<ReadingState>(*this);
}

bool NVM::State::ReadingState::read(const Address& address,
                                    const RowData& data) {
    return true;
}

bool NVM::State::ReadingState::write(const Address& address,
                                     const RowData& data) {
    return true;
}

bool NVM::State::ReadingState::activate(const Address& address) { return true; }

bool NVM::State::ReadingState::precharge(const Address& address) {
    return true;
}

bool NVM::State::ReadingState::refresh() { return true; }
