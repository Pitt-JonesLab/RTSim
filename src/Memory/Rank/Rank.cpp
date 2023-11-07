#include "Memory/Rank/Rank.h"

using namespace NVM::Memory;

Command* Rank::issue(Instruction& instruction) {
    if (banks.empty()) return nullptr;
    return banks[0]->issue(instruction);
}

void Rank::addBank(std::unique_ptr<Bank> bank) {
    banks.emplace_back(std::move(bank));
}