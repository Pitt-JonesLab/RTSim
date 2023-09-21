#include "traceSim/TraceSimulator.h"

#include "include/NVMainRequest.h"
#include "src/MemoryComponent.h"
#include "src/NVMObject.h"
#include "traceReader/GenericTraceReader.h"
#include "traceReader/TraceLine.h"

#include <iostream>
#include <memory>

using namespace NVM;

TraceMain::TraceMain(MemoryComponent* memory, bool ignoreData,
                     bool ignoreReqCycle, GenericTraceReader* traceReader,
                     ncycle_t maxCycles, std::ostream& statStream) :
    memorySystem(memory),
    ignoreData(ignoreData),
    ignoreReqCycle(ignoreReqCycle),
    trace(traceReader),
    simulateCycles(maxCycles),
    statStream(statStream),
    currentCycle(0) {}

NVMainRequest* TraceMain::getNextRequest() {
    TraceLine tl;
    if (!trace->GetNextAccess(&tl)) return nullptr;

    NVMainRequest* request = new NVMainRequest();
    request->address = tl.GetAddress();
    request->address2 = tl.GetAddress2();
    request->type = tl.GetOperation();
    request->bulkCmd = CMD_NOP;
    request->threadId = tl.GetThreadId();
    request->status = MEM_REQUEST_INCOMPLETE;
    if (!ignoreData) request->data = tl.GetData();
    if (!ignoreData) request->oldData = tl.GetOldData();
    request->owner = dynamic_cast<NVMObject*>(memorySystem);
    request->arrivalCycle = tl.GetCycle();

    return request;
}

void TraceMain::waitForDrain() {
    std::cout << "TraceMain - end of trace file reached\n";
    std::cout << "Could not read next line from trace file!" << std::endl;

    while (!memorySystem->isEmpty()) {
        if (!tryCycle(1)) return;
    }
}

bool TraceMain::issueRequest(NVMainRequest* req) {
    while (!memorySystem->isIssuable(req)) {
        if (!tryCycle(1)) return false;
    }

    auto requestCycle = req->arrivalCycle;
    std::cout << "TraceMain - Issuing request " << requestCycle << " at cycle "
              << currentCycle << std::endl;
    memorySystem->issue(req);

    return true;
}

bool TraceMain::tryCycle(ncycle_t cycles) {
    bool result = true;
    if (simulateCycles < (currentCycle + cycles) && simulateCycles > 0) {
        cycles = simulateCycles - currentCycle;
        result = false;
    }
    memorySystem->cycle(cycles);
    currentCycle += cycles;
    return result;
}

bool TraceMain::issueRowClone(NVMainRequest* req) {
    NVMainRequest* srcReq = req;
    srcReq->type = ROWCLONE_SRC;
    if (!issueRequest(srcReq)) return false;

    NVMainRequest* destReq = new NVMainRequest();
    *destReq = *req;
    destReq->address = destReq->address2;
    destReq->type = ROWCLONE_DEST;
    if (!issueRequest(destReq)) return false;
    return true;
}

void TraceMain::runSimulation() {
    std::cout << "*** Simulating " << simulateCycles << " memory cycles"
              << std::endl;

    while (currentCycle <= simulateCycles || simulateCycles == 0) {
        std::cout << "TraceMain - Current cycle: " << currentCycle << std::endl;

        auto request = getNextRequest();

        if (!request) {
            waitForDrain();
            break;
        }

        auto requestCycle = request->arrivalCycle;

        if (request->type != READ && request->type != WRITE &&
            request->type != PIM_OP)
            throw std::runtime_error("Unknown operation in trace file");

        if (requestCycle > currentCycle && !ignoreReqCycle)
            if (!tryCycle(requestCycle - currentCycle)) break;

        if (request->type == PIM_OP) {
            if (!issueRowClone(request)) break;
        } else {
            if (!issueRequest(request)) break;
        }
    }
}

void TraceMain::printStats() {
    memorySystem->printStats(statStream);
    std::cout << "Exiting at cycle " << currentCycle << " because simCycles "
              << simulateCycles << " reached." << std::endl;
}

int TraceMain::runTrace() {
    runSimulation();
    printStats();
    return 0;
}