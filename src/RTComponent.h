#pragma once

#include "src/MemoryComponent.h"
#include "src/NVMObject.h"

namespace NVM {

class NVMObject;
class Config;

/**
 * Placeholder class while switching to MemoryComponents
 *
 * Holds the entire NVMain system
 */
class RTComponent : public MemoryComponent, public NVMObject {
    public:
    RTComponent() = default;
    RTComponent(Config* config);

    void cycle(unsigned int cycles) final;
    bool isIssuable(NVMainRequest* req) final;
    bool issue(NVMainRequest* req) final;
    bool isEmpty() final;
    void printStats(std::ostream& statStream) final;

    void Cycle(ncycle_t cycles);
    bool IsIssuable(NVMainRequest* req, FailReason* reason = nullptr);
    bool IssueCommand(NVMainRequest* req);
    bool Drain();
    bool RequestComplete(NVMainRequest* req);

    private:
    int outstandingRequests;
};

} // namespace NVM