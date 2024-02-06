#include "MemoryTypes/State/StateBank.h"

#include "MemoryTypes/State/ClosedState.h"

using namespace NVM::State;

StateBank::StateBank() { currentState = std::make_unique<ClosedState>(); }

/*
 *   Performs a READ command.
 */
bool StateBank::read(const Address& address, const RowData& data) {
    return currentState->read(address, data);
}

/*
 *   Performs a WRITE command.
 */
bool StateBank::write(const Address& address, const RowData& data) {
    return currentState->write(address, data);
}

/*
 *   Performs an ACTIVATE command.
 */
bool StateBank::activate(const Address& address) {
    return currentState->activate(address);
}

/*
 *   Performs a PRECHARGE command.
 */
bool StateBank::precharge(const Address& address) {
    return currentState->precharge(address);
}

/*
 *   Performs a REFRESH command.
 */
bool StateBank::refresh() { return currentState->refresh(); }

void NVM::State::StateBank::cycle() {
    currentState->cycle();
    if (currentState->finished()) {
        auto s = currentState->getStats();
        stats += s;
        currentState = currentState->getNext();
    }
}

NVM::Stats::ValueStatBlock NVM::State::StateBank::getStats() const {
    return stats;
}
