#include "Memory/MemoryFactory.h"

#include "MemoryTypes/Component/System/ComponentSystem.h"
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
    int numCorrectableFaults;
    int wordSize;
};

std::unique_ptr<SubArray> makeSimpleSubArray() {
    SimpleConfigs configs;

    ConfigParser::registerValue<double>("PIMFaultRate", 0.0,
                                        &configs.pimFaultRate);
    ConfigParser::registerValue<int>("NumTries", 1, &configs.numTries);
    ConfigParser::registerValue<int>("NumCorrectableFaults", 0,
                                     &configs.numCorrectableFaults);
    ConfigParser::registerValue<int>("WordSize", 64, &configs.wordSize);

    FaultModel model(configs.pimFaultRate, configs.wordSize,
                     configs.numCorrectableFaults);

    int dbcs;
    ConfigParser::registerValue<int>("DBCS", 64, &dbcs);

    auto timer = std::make_unique<NVM::Timing::ConfigurableTimer>();
    return std::unique_ptr<SubArray>(
        new SimpleSubArray(dbcs, std::move(timer), model, configs.numTries));
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

std::unique_ptr<MemorySystem> NVM::Memory::makeSimpleSystem() {
    SimpleSystem* system = new SimpleSystem();
    system->addController(makeSimpleController());
    return std::unique_ptr<MemorySystem>(system);
}

using NVM::State::StateSystem;

std::unique_ptr<MemorySystem> NVM::Memory::makeStateSystem() {
    return std::make_unique<StateSystem>();
}

using NVM::ComponentType::ComponentSystem;

std::unique_ptr<MemorySystem> NVM::Memory::makeComponentSystem() {
    return std::make_unique<ComponentSystem>();
}