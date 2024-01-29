#include "Memory/MemoryFactory.h"

#include "MemoryTypes/Simple/Bank/SimpleBank.h"
#include "MemoryTypes/Simple/Interconnect/SimpleInterconnect.h"
#include "MemoryTypes/Simple/MemoryController/SimpleController.h"
#include "MemoryTypes/Simple/MemorySystem/SimpleSystem.h"
#include "MemoryTypes/Simple/Rank/SimpleRank.h"
#include "MemoryTypes/Simple/SubArray/SimpleSubArray.h"
#include "MemoryTypes/Simple/Timing/Timer/ConfigurableTimer.h"
#include "MemoryTypes/State/StateSystem.h"
#include "Utils/ConfigParser.h"

using namespace NVM::Memory;

struct SimpleConfigs {
    double pimFaultRate;
    int numTries;
};

std::unique_ptr<SubArray>
makeSimpleSubArray(const NVM::Simulation::Config& conf) {
    SimpleConfigs configs;

    ConfigParser parser;
    parser.registerValue<double>("PIMFaultRate", 0.0, &configs.pimFaultRate);
    parser.registerValue<int>("NumTries", 1, &configs.numTries);

    parser.parse(conf);

    FaultModel model(configs.pimFaultRate);

    auto timer = std::make_unique<NVM::Timing::ConfigurableTimer>(conf);
    return std::unique_ptr<SubArray>(new SimpleSubArray(
        conf.get<int>("DBCS"), std::move(timer), model, configs.numTries));
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

std::unique_ptr<MemorySystem>
NVM::Memory::makeSimpleSystem(const NVM::Simulation::Config& conf) {
    SimpleSystem* system = new SimpleSystem();
    system->addController(makeSimpleController(conf));
    return std::unique_ptr<MemorySystem>(system);
}

using NVM::State::StateSystem;

std::unique_ptr<MemorySystem>
NVM::Memory::makeStateSystem(const NVM::Simulation::Config& conf) {
    return std::make_unique<StateSystem>();
}