#include "MemoryTypes/State/WritingState.h"

#include "MemoryTypes/State/ClosedState.h"

using namespace NVM::State;

NVM::State::WritingState::WritingState() : State(), remainingCycles(5) {}

void NVM::State::WritingState::cycle() {
    if (remainingCycles) remainingCycles--;
}

std::unique_ptr<State> NVM::State::WritingState::getNext() const {
    if (remainingCycles) return nullptr;
    return std::make_unique<ClosedState>();
}

bool NVM::State::WritingState::read(const Address& address,
                                    const RowData& data) {
    return deny();
}

bool NVM::State::WritingState::write(const Address& address,
                                     const RowData& data) {
    return deny();
}

bool NVM::State::WritingState::activate(const Address& address) {
    return except();
}

bool NVM::State::WritingState::precharge(const Address& address) {
    return deny();
}

bool NVM::State::WritingState::refresh() { return except(); }

bool NVM::State::WritingState::finished() const { return !remainingCycles; }
