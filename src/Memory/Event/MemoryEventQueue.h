#pragma once

#include "Memory/MemorySystem/MemorySystem.h"
#include "Memory/Event/CommandEventQueue.h"

namespace NVM::Memory {

using MemoryEventQueue = CommandEventQueue<NVM::Memory::MemorySystem>;

}