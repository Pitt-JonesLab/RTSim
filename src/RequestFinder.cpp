#include "src/RequestFinder.h"

#include "src/EventQueue.h"
#include "src/MemoryController.h"
#include "src/NVMObject.h"
#include "src/SubArray.h"

using namespace NVM;

RequestFinder::RequestFinder(MemoryController* owner) : owner(owner) {}

NVMainRequest* RequestFinder::searchForTransaction(
    std::list<NVMainRequest*>& transactionQueue,
    std::function<bool(NVMainRequest*)> filter) {
    auto search =
        std::find_if(transactionQueue.begin(), transactionQueue.end(), filter);
    if (search != transactionQueue.end()) {
        auto req = *search;
        if (owner->IsLastRequest(transactionQueue, req))
            req->flags |= NVMainRequest::FLAG_LAST_REQUEST;
        transactionQueue.erase(search);
        return req;
    }
    return nullptr;
}

bool RequestFinder::FindStarvedRequest(
    std::list<NVMainRequest*>& transactionQueue,
    NVMainRequest** starvedRequest) {
    SchedulingPredicate pred;

    return FindStarvedRequest(transactionQueue, starvedRequest, pred);
}

bool RequestFinder::FindStarvedRequest(
    std::list<NVMainRequest*>& transactionQueue, NVMainRequest** starvedRequest,
    SchedulingPredicate& pred) {
    auto isStarved = [&](NVMainRequest* req) {
        ncounter_t queueId = owner->GetCommandQueueId(req->address);
        ncounter_t rank, bank, row, subarray, col;
        req->address.GetTranslatedAddress(&row, &col, &bank, &rank, NULL,
                                          &subarray);
        ncounter_t muxLevel = static_cast<ncounter_t>(col / owner->p->RBSize);

        return (owner->bankActivated[req] && (!owner->rowIsActivated(req)) &&
                !owner->bankNeedRefresh[rank][bank] &&
                !owner->refreshQueued[rank][bank] &&
                owner->starvationCounters[req] >= owner->starvationThreshold &&
                req->arrivalCycle !=
                    owner->GetEventQueue()->GetCurrentCycle() &&
                owner->commandQueues.isEmpty(req->address) && pred(req));
    };

    *starvedRequest = searchForTransaction(transactionQueue, isStarved);
    return (*starvedRequest != nullptr);
}

bool RequestFinder::FindCachedAddress(
    std::list<NVMainRequest*>& transactionQueue,
    NVMainRequest** accessibleRequest) {
    SchedulingPredicate pred;

    return FindCachedAddress(transactionQueue, accessibleRequest, pred);
}

bool RequestFinder::FindCachedAddress(
    std::list<NVMainRequest*>& transactionQueue,
    NVMainRequest** accessibleRequest, SchedulingPredicate& pred) {
    auto isAccessible = [&](NVMainRequest* req) {
        if (req->type == ROWCLONE_SRC || req->type == ROWCLONE_DEST)
            return false;

        ncounter_t queueId = owner->GetCommandQueueId(req->address);
        NVMainRequest* cachedRequest = owner->reqMaker.makeCachedRequest(req);

        bool good =
            (owner->commandQueues.isEmpty(req->address) &&
             owner->GetChild()->IsIssuable(cachedRequest) &&
             req->arrivalCycle != owner->GetEventQueue()->GetCurrentCycle() &&
             pred(req));

        delete cachedRequest;
        return good;
    };

    *accessibleRequest = searchForTransaction(transactionQueue, isAccessible);
    return (*accessibleRequest != nullptr);
}

bool RequestFinder::FindWriteStalledRead(
    std::list<NVMainRequest*>& transactionQueue, NVMainRequest** hitRequest) {
    SchedulingPredicate pred;

    return FindWriteStalledRead(transactionQueue, hitRequest, pred);
}

bool RequestFinder::FindWriteStalledRead(
    std::list<NVMainRequest*>& transactionQueue, NVMainRequest** hitRequest,
    SchedulingPredicate& pred) {
    auto isHit = [&](NVMainRequest* req) {
        if (!owner->p->WritePausing) return false;

        ncounter_t rank, bank, row, subarray, col;
        ncounter_t queueId = owner->GetCommandQueueId(req->address);
        req->address.GetTranslatedAddress(&row, &col, &bank, &rank, NULL,
                                          &subarray);
        SubArray* writingArray = dynamic_cast<SubArray*>(
            owner->_FindChild(req, typeid(SubArray).name()));

        if (writingArray == NULL) return false;
        if (!writingArray->BetweenWriteIterations() &&
            owner->p->pauseMode == PauseMode_Normal) {
            return false;
        }

        NVMainRequest* testActivate = owner->reqMaker.makeActivateRequest(req);
        testActivate->flags |= NVMainRequest::FLAG_PRIORITY;

        bool good =
            (req->type == READ && !owner->bankNeedRefresh[rank][bank] &&
             !owner->refreshQueued[rank][bank] && writingArray->IsWriting() &&
             (owner->GetChild()->IsIssuable(req) ||
              owner->GetChild()->IsIssuable(testActivate)) &&
             req->arrivalCycle != owner->GetEventQueue()->GetCurrentCycle() &&
             owner->commandQueues.isEmpty(req->address) && pred(req));

        delete testActivate;
        return good;
    };

    *hitRequest = searchForTransaction(transactionQueue, isHit);
    return (*hitRequest != nullptr);
}

bool RequestFinder::FindRowBufferHit(
    std::list<NVMainRequest*>& transactionQueue, NVMainRequest** hitRequest) {
    SchedulingPredicate pred;

    return FindRowBufferHit(transactionQueue, hitRequest, pred);
}

bool RequestFinder::FindRowBufferHit(
    std::list<NVMainRequest*>& transactionQueue, NVMainRequest** hitRequest,
    SchedulingPredicate& pred) {
    auto isHit = [&](NVMainRequest* req) {
        ncounter_t rank, bank, row, subarray, col;
        ncounter_t queueId = owner->GetCommandQueueId(req->address);
        req->address.GetTranslatedAddress(&row, &col, &bank, &rank, NULL,
                                          &subarray);
        ncounter_t muxLevel = static_cast<ncounter_t>(col / owner->p->RBSize);

        return (owner->bankActivated[req] && owner->rowIsActivated(req) &&
                !owner->bankNeedRefresh[rank][bank] &&
                !owner->refreshQueued[rank][bank] &&
                req->arrivalCycle !=
                    owner->GetEventQueue()->GetCurrentCycle() &&
                owner->commandQueues.isEmpty(req->address) && pred(req));
    };
    *hitRequest = searchForTransaction(transactionQueue, isHit);

    return (*hitRequest != nullptr);
}

bool RequestFinder::FindRTMRowBufferHit(
    std::list<NVMainRequest*>& transactionQueue, NVMainRequest** hitRequest) {
    SchedulingPredicate pred;

    return FindRTMRowBufferHit(transactionQueue, hitRequest, pred);
}

bool RequestFinder::FindRTMRowBufferHit(
    std::list<NVMainRequest*>& transactionQueue, NVMainRequest** hitRequest,
    SchedulingPredicate& pred) {
    auto isHit = [&](NVMainRequest* req) {
        ncounter_t rank, bank, row, subarray, col;
        ncounter_t queueId = owner->GetCommandQueueId(req->address);
        req->address.GetTranslatedAddress(&row, &col, &bank, &rank, NULL,
                                          &subarray);
        ncounter_t muxLevel = static_cast<ncounter_t>(col / owner->p->RBSize);

        return (owner->bankActivated[req] && owner->rowIsActivated(req) &&
                !owner->bankNeedRefresh[rank][bank] &&
                !owner->refreshQueued[rank][bank] &&
                req->arrivalCycle !=
                    owner->GetEventQueue()->GetCurrentCycle() &&
                owner->commandQueues.isEmpty(req->address) && pred(req));
    };

    *hitRequest = searchForTransaction(transactionQueue, isHit);
    return (*hitRequest != nullptr);
}

bool RequestFinder::FindOldestReadyRequest(
    std::list<NVMainRequest*>& transactionQueue,
    NVMainRequest** oldestRequest) {
    SchedulingPredicate pred;

    return FindOldestReadyRequest(transactionQueue, oldestRequest, pred);
}

bool RequestFinder::FindOldestReadyRequest(
    std::list<NVMainRequest*>& transactionQueue, NVMainRequest** oldestRequest,
    SchedulingPredicate& pred) {
    auto isOldest = [&](NVMainRequest* req) {
        ncounter_t rank, bank;
        ncounter_t queueId = owner->GetCommandQueueId(req->address);
        req->address.GetTranslatedAddress(NULL, NULL, &bank, &rank, NULL, NULL);

        return (
            owner->bankActivated[req] && !owner->bankNeedRefresh[rank][bank] &&
            !owner->refreshQueued[rank][bank] &&
            owner->commandQueues.isEmpty(req->address) &&
            req->arrivalCycle != owner->GetEventQueue()->GetCurrentCycle() &&
            pred(req));
    };

    *oldestRequest = searchForTransaction(transactionQueue, isOldest);
    return (*oldestRequest != nullptr);
}

bool RequestFinder::FindClosedBankRequest(
    std::list<NVMainRequest*>& transactionQueue,
    NVMainRequest** closedRequest) {
    SchedulingPredicate pred;

    return FindClosedBankRequest(transactionQueue, closedRequest, pred);
}

bool RequestFinder::FindClosedBankRequest(
    std::list<NVMainRequest*>& transactionQueue, NVMainRequest** closedRequest,
    SchedulingPredicate& pred) {
    auto isClosed = [&](NVMainRequest* req) {
        ncounter_t rank, bank;
        ncounter_t queueId = owner->GetCommandQueueId(req->address);
        req->address.GetTranslatedAddress(NULL, NULL, &bank, &rank, NULL, NULL);

        return (
            !owner->bankActivated[req] && !owner->bankNeedRefresh[rank][bank] &&
            !owner->refreshQueued[rank][bank] &&
            owner->commandQueues.isEmpty(req->address) &&
            req->arrivalCycle != owner->GetEventQueue()->GetCurrentCycle() &&
            pred(req));
    };

    *closedRequest = searchForTransaction(transactionQueue, isClosed);
    return (*closedRequest != nullptr);
}