#include "MemoryTypes/State/OpenState.h"

using namespace NVM::State;

NVM::State::OpenState::OpenState() {}

void NVM::State::OpenState::cycle() {}

std::unique_ptr<State> NVM::State::OpenState::getNext() const {
    return nullptr;
}

bool NVM::State::OpenState::read(const Address& address, const RowData& data) {
    return true;
}

bool NVM::State::OpenState::write(const Address& address, const RowData& data) {
    return true;
}

bool NVM::State::OpenState::activate(const Address& address) { return true; }

bool NVM::State::OpenState::precharge(const Address& address) { return true; }

bool NVM::State::OpenState::refresh() { return true; }
