#include "Memory/MemoryController/SimpleController.h"

using namespace NVM::Memory;

Command* SimpleController::read(uint64_t address,
                                NVM::Simulation::DataBlock data) {
    return nullptr;
}

Command* SimpleController::write(uint64_t address,
                                 NVM::Simulation::DataBlock data) {
    return nullptr;
}

void SimpleController::cycle(unsigned int cycles) {}

bool SimpleController::isEmpty() const { return false; }

void SimpleController::addInterconnect(
    std::unique_ptr<Interconnect> interconnect) {}