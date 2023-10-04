#include "src/main/RTSystem.h"

#include "NVM/nvmain.h"
#include "SimInterface/NullInterface/NullInterface.h"
#include "src/EventQueue.h"
#include "Utils/HookFactory.h"

using namespace NVM;
using namespace NVM::Simulation;
using namespace NVM::Main;

RTSystem::RTSystem(NVM::Config* config) : outstandingRequests(0) {
    SetEventQueue(new EventQueue());
    SetGlobalEventQueue(new GlobalEventQueue());
    SetStats(new Stats());
    SetTagGenerator(new TagGenerator(1000));

    SimInterface* simInterface = new NullInterface();
    config->SetSimInterface(simInterface);
    simInterface->SetConfig(config, true);

    std::vector<std::string>& hookList = config->GetHooks();

    for (size_t i = 0; i < hookList.size(); i++) {
        std::cout << "Creating hook " << hookList[i] << std::endl;

        NVMObject* hook = HookFactory::CreateHook(hookList[i]);

        if (hook != NULL) {
            AddHook(hook);
            hook->SetParent(this);
            hook->Init(config);
        } else {
            std::cout << "Warning: Could not create a hook named `"
                      << hookList[i] << "'." << std::endl;
        }
    }

    NVMain* nvmain = new NVMain();
    AddChild(nvmain);
    nvmain->SetParent(this);

    globalEventQueue->SetFrequency(config->GetEnergy("CPUFreq") * 1000000.0);
    globalEventQueue->AddSystem(nvmain, config);

    nvmain->SetConfig(config, "defaultMemory", true);

    std::cout << "traceMain (" << (void*) (this) << ")" << std::endl;
    nvmain->PrintHierarchy();
}

bool RTSystem::issue(NVMainRequest* req) {
    if (!GetChild()->IsIssuable(req, nullptr)) return false;
    req->owner = this;
    outstandingRequests++;
    return GetChild()->IssueCommand(req);
}

void RTSystem::cycle(unsigned int cycles) { globalEventQueue->Cycle(cycles); }

bool RTSystem::isEmpty() const { return outstandingRequests == 0; }

bool RTSystem::RequestComplete(NVMainRequest* req) {
    outstandingRequests--;
    delete req;
    return true;
}

void RTSystem::printStats(std::ostream& statStream) {
    GetChild()->CalculateStats();
    GetStats()->PrintAll(statStream);
}