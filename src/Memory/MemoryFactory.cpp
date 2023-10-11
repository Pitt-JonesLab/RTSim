#include "Memory/MemoryFactory.h"

#include "Memory/MemorySystem.h"
#include "Memory/MemorySystem/SimpleSystem.h"

using namespace NVM::Memory;

std::unique_ptr<MemorySystem> NVM::Memory::makeSimpleSystem() {
    return std::unique_ptr<MemorySystem>(new SimpleSystem());
}