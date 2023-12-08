#include "Memory/Rank/Rank.h"

using namespace NVM::Memory;

bool Rank::issue(NVM::Command cmd) { return banks[0]->issue(cmd); }

void Rank::addBank(std::unique_ptr<Bank> bank) {
    banks.emplace_back(std::move(bank));
}