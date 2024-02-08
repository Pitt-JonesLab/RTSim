#include "MemoryTypes/Component/WritingState.h"

using namespace NVM::ComponentType;

NVM::ComponentType::WritingState::WritingState(
    Connection<BankCommand>* cmd, Connection<BankResponse>* response) :
    BankState(cmd, response) {}

void NVM::ComponentType::WritingState::process() {}

void NVM::ComponentType::WritingState::cycle() {}

std::unique_ptr<BankState> NVM::ComponentType::WritingState::nextState() {
    return std::move(next);
}
