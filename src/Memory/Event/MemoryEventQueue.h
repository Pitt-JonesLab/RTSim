#pragma once

#include "Simulation/Commandable.h"
#include "Memory/Event/CommandEventQueue.h"

namespace NVM::Memory {

using MemoryEventQueue = CommandEventQueue<NVM::Simulation::Commandable>;

}