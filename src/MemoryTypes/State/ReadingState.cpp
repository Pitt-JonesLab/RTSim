#include "MemoryTypes/State/ReadingState.h"

using namespace NVM::State;

NVM::State::ReadingState::ReadingState() : State() {}

void NVM::State::ReadingState::cycle() {}

std::unique_ptr<State> NVM::State::ReadingState::getNext() const {
    return nullptr;
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
