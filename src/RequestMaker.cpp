#include "src/RequestMaker.h"

#include "src/EventQueue.h"

using namespace NVM;

RequestMaker::RequestMaker(NVMObject* owner) : owner(owner) {}

NVMainRequest* RequestMaker::makeCachedRequest(NVMainRequest* triggerRequest) {
    // assert(triggerRequest->type == READ || triggerRequest->type == WRITE);

    NVMainRequest* cachedRequest = new NVMainRequest();

    *cachedRequest = *triggerRequest;
    cachedRequest->type =
        (triggerRequest->type == READ ? CACHED_READ : CACHED_WRITE);
    cachedRequest->owner = owner;

    return cachedRequest;
}

NVMainRequest*
RequestMaker::makeActivateRequest(NVMainRequest* triggerRequest) {
    NVMainRequest* activateRequest = new NVMainRequest();

    activateRequest->type = ACTIVATE;
    activateRequest->issueCycle = owner->GetEventQueue()->GetCurrentCycle();
    activateRequest->address = triggerRequest->address;
    activateRequest->owner = owner;

    return activateRequest;
}

NVMainRequest* RequestMaker::makeShiftRequest(NVMainRequest* triggerRequest) {
    NVMainRequest* shiftRequest = new NVMainRequest();

    shiftRequest->type = SHIFT;
    shiftRequest->issueCycle = owner->GetEventQueue()->GetCurrentCycle();
    shiftRequest->address = triggerRequest->address;
    shiftRequest->owner = owner;

    return shiftRequest;
}

NVMainRequest*
RequestMaker::makePrechargeRequest(NVMainRequest* triggerRequest) {
    NVMainRequest* prechargeRequest = new NVMainRequest();

    prechargeRequest->type = PRECHARGE;
    prechargeRequest->issueCycle = owner->GetEventQueue()->GetCurrentCycle();
    prechargeRequest->address = triggerRequest->address;
    prechargeRequest->owner = owner;

    return prechargeRequest;
}

NVMainRequest* RequestMaker::makePrechargeRequest(const ncounter_t row,
                                                  const ncounter_t col,
                                                  const ncounter_t bank,
                                                  const ncounter_t rank,
                                                  const ncounter_t channel,
                                                  const ncounter_t subarray) {
    NVMainRequest* prechargeRequest = new NVMainRequest();

    prechargeRequest->type = PRECHARGE;
    ncounter_t preAddr = owner->GetDecoder()->ReverseTranslate(
        row, col, bank, rank, channel, subarray);
    prechargeRequest->address.SetPhysicalAddress(preAddr);
    prechargeRequest->address.SetTranslatedAddress(row, col, bank, rank,
                                                   channel, subarray);
    prechargeRequest->issueCycle = owner->GetEventQueue()->GetCurrentCycle();
    prechargeRequest->owner = owner;

    return prechargeRequest;
}

NVMainRequest* RequestMaker::makePrechargeAllRequest(
    const ncounter_t row, const ncounter_t col, const ncounter_t bank,
    const ncounter_t rank, const ncounter_t channel,
    const ncounter_t subarray) {
    NVMainRequest* prechargeAllRequest = new NVMainRequest();

    prechargeAllRequest->type = PRECHARGE_ALL;
    ncounter_t preAddr = owner->GetDecoder()->ReverseTranslate(
        row, col, bank, rank, channel, subarray);
    prechargeAllRequest->address.SetPhysicalAddress(preAddr);
    prechargeAllRequest->address.SetTranslatedAddress(row, col, bank, rank,
                                                      channel, subarray);
    prechargeAllRequest->issueCycle = owner->GetEventQueue()->GetCurrentCycle();
    prechargeAllRequest->owner = owner;

    return prechargeAllRequest;
}

NVMainRequest*
RequestMaker::makeImplicitPrechargeRequest(NVMainRequest* triggerRequest) {
    if (triggerRequest->type == READ) triggerRequest->type = READ_PRECHARGE;
    else if (triggerRequest->type == WRITE)
        triggerRequest->type = WRITE_PRECHARGE;

    triggerRequest->issueCycle = owner->GetEventQueue()->GetCurrentCycle();

    return triggerRequest;
}

NVMainRequest* RequestMaker::makeRefreshRequest(const ncounter_t row,
                                                const ncounter_t col,
                                                const ncounter_t bank,
                                                const ncounter_t rank,
                                                const ncounter_t channel,
                                                const ncounter_t subarray) {
    NVMainRequest* refreshRequest = new NVMainRequest();

    refreshRequest->type = REFRESH;
    ncounter_t preAddr = owner->GetDecoder()->ReverseTranslate(
        row, col, bank, rank, channel, subarray);
    refreshRequest->address.SetPhysicalAddress(preAddr);
    refreshRequest->address.SetTranslatedAddress(row, col, bank, rank, channel,
                                                 subarray);
    refreshRequest->issueCycle = owner->GetEventQueue()->GetCurrentCycle();
    refreshRequest->owner = owner;

    return refreshRequest;
}

NVMainRequest* RequestMaker::makePowerdownRequest(OpType pdOp,
                                                  const ncounter_t rank,
                                                  const ncounter_t channel) {
    NVMainRequest* powerdownRequest = new NVMainRequest();

    powerdownRequest->type = pdOp;
    ncounter_t pdAddr =
        owner->GetDecoder()->ReverseTranslate(0, 0, 0, rank, channel, 0);
    powerdownRequest->address.SetPhysicalAddress(pdAddr);
    powerdownRequest->address.SetTranslatedAddress(0, 0, 0, rank, channel, 0);
    powerdownRequest->issueCycle = owner->GetEventQueue()->GetCurrentCycle();
    powerdownRequest->owner = owner;

    return powerdownRequest;
}

NVMainRequest* RequestMaker::makePowerupRequest(const ncounter_t rank,
                                                const ncounter_t channel) {
    NVMainRequest* powerupRequest = new NVMainRequest();

    powerupRequest->type = POWERUP;
    ncounter_t puAddr =
        owner->GetDecoder()->ReverseTranslate(0, 0, 0, rank, channel, 0);
    powerupRequest->address.SetPhysicalAddress(puAddr);
    powerupRequest->address.SetTranslatedAddress(0, 0, 0, rank, channel, 0);
    powerupRequest->issueCycle = owner->GetEventQueue()->GetCurrentCycle();
    powerupRequest->owner = owner;

    return powerupRequest;
}