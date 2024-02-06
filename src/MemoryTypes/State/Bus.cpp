#include "MemoryTypes/State/Bus.h"

#include <iostream>

NVM::State::Bus::Bus() : bank(nullptr), currentState(BUS_STATE::NO_OP) {}

void NVM::State::Bus::setBank(StateBank* b) { bank = b; }

bool NVM::State::Bus::read(const Address& address, const RowData& data) {
    if (currentState != BUS_STATE::NO_OP) return except();
    currentState = BUS_STATE::READ;
    if (bank) return bank->read(address, data);
    return false;
}

bool NVM::State::Bus::write(const Address& address, const RowData& data) {
    if (currentState != BUS_STATE::NO_OP) return except();
    currentState = BUS_STATE::WRITE;
    if (bank) return bank->write(address, data);
    return false;
}

bool NVM::State::Bus::activate(const Address& address) {
    if (currentState != BUS_STATE::NO_OP) return except();
    if (bank) return bank->activate(address);
    return false;
}

bool NVM::State::Bus::precharge(const Address& address) {
    if (currentState != BUS_STATE::NO_OP) return except();
    if (bank) return bank->precharge(address);
    return false;
}

bool NVM::State::Bus::refresh() {
    if (currentState != BUS_STATE::NO_OP) return except();
    if (bank) return bank->refresh();
    return false;
}

bool NVM::State::Bus::sendData() {
    if (currentState != BUS_STATE::NO_OP) return except();
    return true;
}

void NVM::State::Bus::cycle() {
    commandHistory.push_back(currentState);
    currentState = BUS_STATE::NO_OP;
    if (bank) bank->cycle();
}

void NVM::State::Bus::printCycles() const {
    for (const auto state : commandHistory) std::cout << state << '\n';
}
