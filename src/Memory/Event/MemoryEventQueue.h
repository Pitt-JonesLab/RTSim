#pragma once

#include "Memory/Commandable.h"
#include "Memory/Event/CommandEventQueue.h"

namespace NVM::Memory {

using MemoryEventQueue = CommandEventQueue<NVM::Memory::Commandable>;

}