#include "MemoryTypes/State/ClosedState.h"

#include "MemoryTypes/State/ReadingState.h"
#include "MemoryTypes/State/WritingState.h"

using namespace NVM::State;

NVM::State::ClosedState::ClosedState() : State() {}

void NVM::State::ClosedState::cycle() {}

std::unique_ptr<State> NVM::State::ClosedState::getNext() const {
    switch (nextState) {
        case NextState::READ:
            return std::make_unique<ReadingState>();
        case NextState::WRITE:
            return std::make_unique<WritingState>();
        default:
            return nullptr;
    }
}

bool NVM::State::ClosedState::read(const Address& address,
                                   const RowData& data) {
    nextState = NextState::READ;
    return true;
}

bool NVM::State::ClosedState::write(const Address& address,
                                    const RowData& data) {
    nextState = NextState::WRITE;
    return true;
}

bool NVM::State::ClosedState::activate(const Address& address) { return true; }

bool NVM::State::ClosedState::precharge(const Address& address) { return true; }

bool NVM::State::ClosedState::refresh() { return true; }

bool NVM::State::ClosedState::finished() const {
    return nextState != NextState::NONE;
}
