#include "MemoryTypes/State/WritingState.h"

using namespace NVM::State;

NVM::State::WritingState::WritingState() : State() {}

std::unique_ptr<State> NVM::State::WritingState::cycle() {
    return std::make_unique<WritingState>(*this);
}

bool NVM::State::WritingState::read(const Address& address,
                                    const RowData& data) {
    return true;
}

bool NVM::State::WritingState::write(const Address& address,
                                     const RowData& data) {
    return true;
}

bool NVM::State::WritingState::activate(const Address& address) { return true; }

bool NVM::State::WritingState::precharge(const Address& address) {
    return true;
}

bool NVM::State::WritingState::refresh() { return true; }
