#include "src/RTComponent.h"

#include "NVM/nvmain.h"
#include "SimInterface/NullInterface/NullInterface.h"
#include "src/EventQueue.h"
#include "Utils/HookFactory.h"

using namespace NVM;

RTComponent::RTComponent(Config* config) {
    outstandingRequests = 0;

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

void RTComponent::Cycle(ncycle_t cycles) { globalEventQueue->Cycle(cycles); }

bool RTComponent::IsIssuable(NVMainRequest* req, FailReason* reason) {
    return GetChild()->IsIssuable(req, reason);
}

bool RTComponent::IssueCommand(NVMainRequest* req) {
    auto requestCycle = req->arrivalCycle;
    std::cout << "TraceMain - Issuing request " << requestCycle << " at cycle "
              << GetEventQueue()->GetCurrentCycle() << std::endl;
    outstandingRequests++;
    return GetChild()->IssueCommand(req);
}

bool RTComponent::Drain() {
    std::cout << "MainObj - Draining with " << outstandingRequests
              << " remaining reqs\n";
    NVMObject::Drain();
    return outstandingRequests == 0;
}

bool RTComponent::RequestComplete(NVMainRequest* req) {
    if (req->owner == this) {
        delete req;
        outstandingRequests--;
        return true;
    }
    throw std::runtime_error("Request with no owner!");
}

void RTComponent::cycle(unsigned int cycles) { Cycle(cycles); }

bool RTComponent::isIssuable(NVMainRequest* req) { return IsIssuable(req); }

bool RTComponent::issue(NVMainRequest* req) { return IssueCommand(req); }

bool RTComponent::isEmpty() { return outstandingRequests == 0; }

void RTComponent::printStats(std::ostream& statStream) {
    GetChild()->CalculateStats();
    GetStats()->PrintAll(statStream);
}