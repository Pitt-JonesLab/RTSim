#include "Memory/MemoryFactory.h"

#include "Memory/Bank/SimpleBank.h"
#include "Memory/Interconnect/SimpleInterconnect.h"
#include "Memory/MemoryController/SimpleController.h"
#include "Memory/MemorySystem/MemorySystem.h"
#include "Memory/MemorySystem/SimpleSystem.h"
#include "Memory/Rank/SimpleRank.h"
#include "Memory/SubArray/SimpleSubArray.h"

using namespace NVM::Memory;

std::unique_ptr<SubArray>
makeSimpleSubArray(const NVM::Simulation::Config& conf) {
    return std::unique_ptr<SubArray>(new SimpleSubArray(conf.get<int>("DBCS")));
}

std::unique_ptr<Bank> makeSimpleBank(const NVM::Simulation::Config& conf) {
    auto bank = std::unique_ptr<Bank>(new SimpleBank());
    bank->addSubArray(makeSimpleSubArray(conf));
    return std::move(bank);
}

std::unique_ptr<Rank> makeSimpleRank(const NVM::Simulation::Config& conf) {
    auto rank = std::unique_ptr<Rank>(new SimpleRank());
    rank->addBank(makeSimpleBank(conf));
    return std::move(rank);
}

std::unique_ptr<Interconnect>
makeSimpleInterconnect(const NVM::Simulation::Config& conf) {
    auto interconnect = std::unique_ptr<Interconnect>(new SimpleInterconnect());
    interconnect->addRank(makeSimpleRank(conf));
    return std::move(interconnect);
}

std::unique_ptr<MemoryController>
makeSimpleController(const NVM::Simulation::Config& conf) {
    auto controller = std::unique_ptr<MemoryController>(new SimpleController());
    controller->addInterconnect(makeSimpleInterconnect(conf));
    return std::move(controller);
}

std::unique_ptr<NVM::Simulation::MemorySystem>
NVM::Memory::makeSimpleSystem(const NVM::Simulation::Config& conf) {
    SimpleSystem* system = new SimpleSystem();
    system->addController(makeSimpleController(conf));
    return std::unique_ptr<NVM::Simulation::MemorySystem>(system);
}