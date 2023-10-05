#pragma once

#include <iostream>

namespace NVM {

struct NVMainRequest;

/**
 * MemoryComponents represent physical components in an RTSim memory system
 *
 * They can handle requests sent between MemoryLayers, either creating responses
 * or propagating the request
 */
class MemoryComponent {
    public:
    MemoryComponent() = default;
    virtual ~MemoryComponent() = default;

    virtual void cycle(unsigned int cycles) = 0;

    // Take NVMainRequests for now
    virtual bool isIssuable(NVMainRequest* req) = 0;

    virtual bool issue(NVMainRequest* req) = 0;

    virtual bool isEmpty() = 0;

    virtual void printStats(std::ostream& statStream) = 0;
};

} // namespace NVM