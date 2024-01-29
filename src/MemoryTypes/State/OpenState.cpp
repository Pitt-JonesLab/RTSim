#include "MemoryTypes/State/OpenState.h"

using namespace NVM::State;

NVM::State::OpenState::OpenState() {}

std::unique_ptr<State> NVM::State::OpenState::cycle() {
    return std::make_unique<OpenState>(*this);
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
