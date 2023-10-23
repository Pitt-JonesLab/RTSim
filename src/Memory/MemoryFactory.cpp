#include "Memory/MemoryFactory.h"

#include "Memory/Bank/SimpleBank.h"
#include "Memory/Interconnect/SimpleInterconnect.h"
#include "Memory/MemoryController/SimpleController.h"
#include "Memory/MemorySystem.h"
#include "Memory/MemorySystem/SimpleSystem.h"
#include "Memory/Rank/SimpleRank.h"
#include "Memory/SubArray/SimpleSubArray.h"

using namespace NVM::Memory;

std::unique_ptr<SubArray> makeSimpleSubArray() {
    return std::unique_ptr<SubArray>(new SimpleSubArray());
}

std::unique_ptr<Bank> makeSimpleBank() {
    auto bank = std::unique_ptr<Bank>(new SimpleBank());
    bank->addSubArray(makeSimpleSubArray());
    return std::move(bank);
}

std::unique_ptr<Rank> makeSimpleRank() {
    auto rank = std::unique_ptr<Rank>(new SimpleRank());
    rank->addBank(makeSimpleBank());
    return std::move(rank);
}

std::unique_ptr<Interconnect> makeSimpleInterconnect() {
    auto interconnect = std::unique_ptr<Interconnect>(new SimpleInterconnect());
    interconnect->addRank(makeSimpleRank());
    return std::move(interconnect);
}

std::unique_ptr<MemoryController> makeSimpleController() {
    auto controller = std::unique_ptr<MemoryController>(new SimpleController());
    controller->addInterconnect(makeSimpleInterconnect());
    return std::move(controller);
}

std::unique_ptr<NVM::Simulation::MemorySystem> NVM::Memory::makeSimpleSystem() {
    SimpleSystem* system = new SimpleSystem();
    system->addController(makeSimpleController());
    return std::unique_ptr<NVM::Simulation::MemorySystem>(system);
}