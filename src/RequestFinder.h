#pragma once

#include "include/NVMainRequest.h"

#include <functional>
#include <list>

namespace NVM {

class SchedulingPredicate {
    public:
    SchedulingPredicate() {}

    ~SchedulingPredicate() {}

    virtual bool operator()(NVMainRequest* /*request*/) { return true; }
};

class MemoryController;

class RequestFinder {
    public:
    RequestFinder(MemoryController* owner);

    bool FindStarvedRequest(std::list<NVMainRequest*>& transactionQueue,
                            NVMainRequest** starvedRequest);
    bool FindCachedAddress(std::list<NVMainRequest*>& transactionQueue,
                           NVMainRequest** accessibleRequest);

    bool FindRowBufferHit(std::list<NVMainRequest*>& transactionQueue,
                          NVMainRequest** hitRequest);
    bool FindRTMRowBufferHit(std::list<NVMainRequest*>& transactionQueue,
                             NVMainRequest** hitRequest);
    bool FindWriteStalledRead(std::list<NVMainRequest*>& transactionQueue,
                              NVMainRequest** hitRequest);
    bool FindOldestReadyRequest(std::list<NVMainRequest*>& transactionQueue,
                                NVMainRequest** oldestRequest);
    bool FindClosedBankRequest(std::list<NVMainRequest*>& transactionQueue,
                               NVMainRequest** closedRequest);
    bool FindStarvedRequest(std::list<NVMainRequest*>& transactionQueue,
                            NVMainRequest** starvedRequest,
                            NVM::SchedulingPredicate& p);
    bool FindRowBufferHit(std::list<NVMainRequest*>& transactionQueue,
                          NVMainRequest** hitRequest,
                          NVM::SchedulingPredicate& p);
    bool FindOldestReadyRequest(std::list<NVMainRequest*>& transactionQueue,
                                NVMainRequest** oldestRequest,
                                NVM::SchedulingPredicate& p);
    bool FindClosedBankRequest(std::list<NVMainRequest*>& transactionQueue,
                               NVMainRequest** closedRequest,
                               NVM::SchedulingPredicate& p);
    bool FindCachedAddress(std::list<NVMainRequest*>& transactionQueue,
                           NVMainRequest** accessibleRequest,
                           NVM::SchedulingPredicate& p);
    bool FindRTMRowBufferHit(std::list<NVMainRequest*>& transactionQueue,
                             NVMainRequest** hitRequest,
                             NVM::SchedulingPredicate& p);
    bool FindWriteStalledRead(std::list<NVMainRequest*>& transactionQueue,
                              NVMainRequest** hitRequest,
                              NVM::SchedulingPredicate& p);

    private:
    MemoryController* owner;

    NVMainRequest*
    searchForTransaction(std::list<NVMainRequest*>& transactionQueue,
                         std::function<bool(NVMainRequest*)> filter);
};

} // namespace NVM