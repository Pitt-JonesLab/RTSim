#pragma once

#include "include/NVMainRequest.h"
#include "src/NVMObject.h"

namespace NVM {

class RequestMaker {
    public:
    RequestMaker();

    NVMainRequest* makeShiftRequest(NVMainRequest* req);
    NVMainRequest* convertToImplicitPrecharge(NVMainRequest* req);
    NVMainRequest* makeActivateRequest(NVMainRequest* req);
    NVMainRequest* makePrechargeRequest(NVMainRequest* req);
    NVMainRequest* makeCachedRequest(NVMainRequest* req);

    NVMainRequest* makeShiftRequest(NVMainRequest* req, NVMObject* owner);
    NVMainRequest* makeImplicitPrechargeRequest(NVMainRequest* req,
                                                NVMObject* owner);
    NVMainRequest* makeActivateRequest(NVMainRequest* req, NVMObject* owner);
    NVMainRequest* makePrechargeRequest(NVMainRequest* req, NVMObject* owner);
    NVMainRequest* makeCachedRequest(NVMainRequest* req, NVMObject* owner);

    NVMainRequest* makePrechargeAllRequest(const NVMAddress& addr);
    NVMainRequest* makeRefreshRequest(const NVMAddress& addr);
};

} // namespace NVM