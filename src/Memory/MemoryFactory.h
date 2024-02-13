#include "Memory/MemorySystem.h"
#include "Simulation/Config.h"

#include <memory>

namespace NVM::Memory {

class MemorySystem;

/**
 * Creates a MemorySystem with all simple parts
 * One Controller, Interconnect, Rank, ...
 *
 * @return Unique pointer to the created MemorySystem
 */
std::unique_ptr<MemorySystem> makeSimpleSystem();

std::unique_ptr<MemorySystem> makeStateSystem();

std::unique_ptr<MemorySystem> makeComponentSystem();

} // namespace NVM::Memory