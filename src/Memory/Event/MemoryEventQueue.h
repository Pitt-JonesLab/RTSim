#pragma once

#include "Memory/Event/CommandEventQueue.h"
#include "Memory/MemorySystem/MemorySystem.h"

namespace NVM::Memory {

using MemoryEventQueue = CommandEventQueue<NVM::Memory::MemorySystem>;

}