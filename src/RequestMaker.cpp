#include "src/RequestMaker.h"

#include "src/EventQueue.h"

using namespace NVM;

RequestMaker::RequestMaker() {}

NVMainRequest* RequestMaker::makeCachedRequest(NVMainRequest* triggerRequest) {
    NVMainRequest* cachedRequest = new NVMainRequest(*triggerRequest);
    cachedRequest->type =
        (triggerRequest->type == READ ? CACHED_READ : CACHED_WRITE);
    return cachedRequest;
}

NVMainRequest*
RequestMaker::makeActivateRequest(NVMainRequest* triggerRequest) {
    NVMainRequest* activateRequest = new NVMainRequest(*triggerRequest);
    activateRequest->type = ACTIVATE;
    return activateRequest;
}

NVMainRequest* RequestMaker::makeShiftRequest(NVMainRequest* triggerRequest) {
    NVMainRequest* shiftRequest = new NVMainRequest(*triggerRequest);
    shiftRequest->type = SHIFT;
    return shiftRequest;
}

NVMainRequest*
RequestMaker::makePrechargeRequest(NVMainRequest* triggerRequest) {
    NVMainRequest* prechargeRequest = new NVMainRequest(*triggerRequest);
    prechargeRequest->type = PRECHARGE;
    return prechargeRequest;
}

NVMainRequest* RequestMaker::makePrechargeAllRequest(const NVMAddress& addr) {
    NVMainRequest* preAllReq = new NVMainRequest();
    preAllReq->type = PRECHARGE_ALL;
    preAllReq->address = addr;
    return preAllReq;
}

NVMainRequest*
RequestMaker::convertToImplicitPrecharge(NVMainRequest* triggerRequest) {
    if (triggerRequest->type == READ) triggerRequest->type = READ_PRECHARGE;
    else if (triggerRequest->type == WRITE)
        triggerRequest->type = WRITE_PRECHARGE;
    return triggerRequest;
}

NVMainRequest* RequestMaker::makeRefreshRequest(const NVMAddress& addr) {
    NVMainRequest* refReq = new NVMainRequest();
    refReq->type = REFRESH;
    refReq->address = addr;
    return refReq;
}

NVMainRequest* RequestMaker::makeShiftRequest(NVMainRequest* req,
                                              NVMObject* owner) {
    auto newReq = makeShiftRequest(req);
    newReq->owner = owner;
    return newReq;
}

NVMainRequest* RequestMaker::makeImplicitPrechargeRequest(NVMainRequest* req,
                                                          NVMObject* owner) {
    auto newReq = convertToImplicitPrecharge(req);
    newReq->owner = owner;
    return newReq;
}

NVMainRequest* RequestMaker::makeActivateRequest(NVMainRequest* req,
                                                 NVMObject* owner) {
    auto newReq = makeActivateRequest(req);
    newReq->owner = owner;
    return newReq;
}

NVMainRequest* RequestMaker::makePrechargeRequest(NVMainRequest* req,
                                                  NVMObject* owner) {
    auto newReq = makePrechargeRequest(req);
    newReq->owner = owner;
    return newReq;
}

NVMainRequest* RequestMaker::makeCachedRequest(NVMainRequest* req,
                                               NVMObject* owner) {
    auto newReq = makeCachedRequest(req);
    newReq->owner = owner;
    return newReq;
}