#include "MemoryTypes/Component/MemoryController.h"

#include "Stats/StatBlock.h"

NVM::ComponentType::MemoryController::MemoryController() :
    commandConnection(nullptr) {}

void NVM::ComponentType::MemoryController::process() {}

void NVM::ComponentType::MemoryController::cycle() {}

NVM::Stats::StatBlock NVM::ComponentType::MemoryController::getStats() {
    return Stats::StatBlock();
}
