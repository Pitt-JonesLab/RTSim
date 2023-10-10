#include "Memory/MemoryFactory.h"

#include "Memory/MemorySystem.h"

using namespace NVM::Memory;

std::unique_ptr<MemorySystem> NVM::Memory::makeSimpleSystem() {
    return nullptr;
}