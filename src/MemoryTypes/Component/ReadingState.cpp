#include "MemoryTypes/Component/ReadingState.h"

using namespace NVM::ComponentType;

NVM::ComponentType::ReadingState::ReadingState(
    Connection<BankCommand>* cmd, Connection<BankResponse>* response) :
    BankState(cmd, response) {}

void NVM::ComponentType::ReadingState::process() {}

void NVM::ComponentType::ReadingState::cycle() {}

std::unique_ptr<BankState> NVM::ComponentType::ReadingState::nextState() {
    return std::move(next);
}
