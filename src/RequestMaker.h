#pragma once

#include "include/NVMainRequest.h"
#include "src/NVMObject.h"

namespace NVM {

class RequestMaker {
    public:
    RequestMaker(NVMObject* owner);

    NVMainRequest* makeShiftRequest(NVMainRequest* triggerRequest);
    NVMainRequest* makeImplicitPrechargeRequest(NVMainRequest* triggerRequest);
    NVMainRequest* makePrechargeRequest(const ncounter_t, const ncounter_t,
                                        const ncounter_t, const ncounter_t,
                                        const ncounter_t, const ncounter_t);
    NVMainRequest* makePrechargeAllRequest(const ncounter_t, const ncounter_t,
                                           const ncounter_t, const ncounter_t,
                                           const ncounter_t, const ncounter_t);
    NVMainRequest* makeRefreshRequest(const ncounter_t, const ncounter_t,
                                      const ncounter_t, const ncounter_t,
                                      const ncounter_t, const ncounter_t);
    NVMainRequest* makePowerdownRequest(OpType pdOp, const ncounter_t rank,
                                        const ncounter_t);
    NVMainRequest* makePowerupRequest(const ncounter_t rank, const ncounter_t);
    NVMainRequest* makeActivateRequest(NVMainRequest* triggerRequest);
    NVMainRequest* makePrechargeRequest(NVMainRequest* triggerRequest);
    NVMainRequest* makeCachedRequest(NVMainRequest* triggerRequest);

    private:
    NVMObject* owner;
};

} // namespace NVM