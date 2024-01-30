#include "MemoryTypes/State/ReadingState.h"

#include "MemoryTypes/State/ClosedState.h"

using namespace NVM::State;

NVM::State::ReadingState::ReadingState() : State(), remainingCycles(5) {}

void NVM::State::ReadingState::cycle() {
    if (remainingCycles) remainingCycles--;
}

std::unique_ptr<State> NVM::State::ReadingState::getNext() const {
    if (remainingCycles) return nullptr;
    return std::make_unique<ClosedState>();
}

bool NVM::State::ReadingState::read(const Address& address,
                                    const RowData& data) {
    return deny();
}

bool NVM::State::ReadingState::write(const Address& address,
                                     const RowData& data) {
    return deny();
}

bool NVM::State::ReadingState::activate(const Address& address) {
    return except();
}

bool NVM::State::ReadingState::precharge(const Address& address) {
    return deny();
}

bool NVM::State::ReadingState::refresh() { return except(); }

bool NVM::State::ReadingState::finished() const { return !remainingCycles; }
