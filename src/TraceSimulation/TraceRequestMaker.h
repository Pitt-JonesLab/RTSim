#pragma once

#include <memory>
#include <vector>

namespace NVM {
class NVMainRequest;
}

namespace NVM::Simulation {

class TraceReader;

/**
 * Converts TraceLines from a TraceReader to NVMainRequests
 */
class TraceRequestMaker {
    public:
    /**
     * Creates a TraceRequestMaker with the given TraceReader
     *
     * @param reader TraceReader for this request maker
     */
    TraceRequestMaker(std::unique_ptr<TraceReader> reader);

    /**
     * Copy constructor. Clones the TraceReader and nextReqs.
     *
     * @param rhs TraceRequestMaker to copy from
     */
    TraceRequestMaker(const TraceRequestMaker& rhs);

    /**
     * Assignment operator. Clones the TraceReader and nextReqs.
     *
     * @param rhs TraceRequestMaker to copy from
     */
    const TraceRequestMaker& operator=(const TraceRequestMaker& rhs);

    /**
     * Gets the next available request
     *
     * @return Next request from the trace. TODO: make NONE type request
     */
    NVM::NVMainRequest* getRequest();

    private:
    std::unique_ptr<TraceReader> reader;
    std::vector<NVMainRequest*> nextReqs;

    /**
     * Gets the next TraceLine from the TraceReaer and translates to one or more
     * requests. Saves the requests in nextReqs.
     */
    void convertNextTraceLine();
};

}; // namespace NVM::Simulation