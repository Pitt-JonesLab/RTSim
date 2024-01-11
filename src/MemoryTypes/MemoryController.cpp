#include "MemoryTypes/MemoryController.h"

using namespace NVM::Memory;

void MemoryController::addInterconnect(
    std::unique_ptr<Interconnect> interconnect) {
    interconnects.emplace_back(std::move(interconnect));
}