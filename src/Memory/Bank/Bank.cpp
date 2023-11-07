#include "Memory/Bank/Bank.h"

using namespace NVM::Memory;

void Bank::addSubArray(std::unique_ptr<SubArray> subArray) {
    subArrays.emplace_back(std::move(subArray));
}

Command* Bank::issue(Instruction& instruction) {
    if (subArrays.empty()) return nullptr;
    return instruction.execute(*subArrays[0].get());
}