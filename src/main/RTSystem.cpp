#include "src/main/RTSystem.h"

#include "src/old/EventQueue.h"
#include "src/old/NVM/nvmain.h"
#include "src/old/SimInterface/NullInterface/NullInterface.h"
#include "src/old/Utils/HookFactory.h"

using namespace NVM;
using namespace NVM::Simulation;
using namespace NVM::Main;

RTSystem::RTSystem(NVM::Config* config) :
    outstandingRequests(0),
    nextRead(nullptr),
    nextWrite(nullptr) {
    SetEventQueue(new EventQueue());
    SetGlobalEventQueue(new GlobalEventQueue());
    SetStats(new Stats());
    SetTagGenerator(new TagGenerator(1000));

    SimInterface* simInterface = new NullInterface();
    config->SetSimInterface(simInterface);
    simInterface->SetConfig(config, true);

    std::vector<std::string>& hookList = config->GetHooks();

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

bool RTSystem::read(uint64_t address, DataBlock data, unsigned int threadId,
                    unsigned int cycle) {
    NVMainRequest* nextRead = new NVMainRequest();

    NVMAddress reqAddress;
    reqAddress.SetPhysicalAddress(address);
    nextRead->address = reqAddress;

    NVMDataBlock reqData;
    reqData.SetSize(64);
    for (int i = 0; i < 64; i++) {
        reqData.SetByte(i, data.bytes[i]);
    }
    nextRead->data = reqData;

    NVMDataBlock oldData;
    oldData.SetSize(64);
    for (int i = 0; i < 64; i++) oldData.SetByte(i, 0);
    nextRead->oldData = oldData;

    nextRead->type = READ;
    nextRead->bulkCmd = CMD_NOP;
    nextRead->threadId = threadId;
    nextRead->arrivalCycle = GetEventQueue()->GetCurrentCycle();
    nextRead->status = MEM_REQUEST_INCOMPLETE;

    bool canIssue = GetChild()->IsIssuable(nextRead);
    if (!canIssue) {
        delete nextRead;
        return false;
    }
    outstandingRequests++;
    GetChild()->IssueCommand(nextRead);
    return canIssue;
}

bool RTSystem::write(uint64_t address, DataBlock data, unsigned int threadId,
                     unsigned int cycle) {
    NVMainRequest* nextWrite = new NVMainRequest();

    NVMAddress reqAddress;
    reqAddress.SetPhysicalAddress(address);
    nextWrite->address = reqAddress;

    NVMDataBlock reqData;
    reqData.SetSize(64);
    for (int i = 0; i < 64; i++) {
        reqData.SetByte(i, data.bytes[i]);
    }
    nextWrite->data = reqData;

    NVMDataBlock oldData;
    oldData.SetSize(64);
    for (int i = 0; i < 64; i++) oldData.SetByte(i, 0);
    nextWrite->oldData = oldData;

    nextWrite->type = WRITE;
    nextWrite->bulkCmd = CMD_NOP;
    nextWrite->threadId = threadId;
    nextWrite->arrivalCycle = GetEventQueue()->GetCurrentCycle();
    nextWrite->status = MEM_REQUEST_INCOMPLETE;
    nextWrite->owner = this;

    bool canIssue = GetChild()->IsIssuable(nextWrite);
    if (!canIssue) {
        delete nextWrite;
        return false;
    }
    outstandingRequests++;
    GetChild()->IssueCommand(nextWrite);
    return canIssue;
}

bool RTSystem::rowClone(uint64_t srcAddress, uint64_t destAddress,
                        DataBlock data, unsigned int threadId,
                        unsigned int cycle) {
    return true;
}

bool RTSystem::transverseRead(uint64_t baseAddress, uint64_t destAddress,
                              std::vector<DataBlock> inputRows,
                              unsigned int threadId, unsigned int cycle) {
    return true;
}

bool RTSystem::transverseWrite(uint64_t baseAddress,
                               std::vector<DataBlock> writeData,
                               unsigned int threadId, unsigned int cycle) {
    return true;
}

bool RTSystem::shift(uint64_t address, unsigned int shiftAmount, DataBlock data,
                     unsigned int threadId, unsigned int cycle) {
    return true;
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

unsigned int RTSystem::getCurrentCycle() {
    return GetEventQueue()->GetCurrentCycle();
}