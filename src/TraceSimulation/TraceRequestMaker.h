#pragma once

#include "src/TraceSimulation/TraceReader.h"

#include <memory>
#include <queue>

namespace NVM {
class NVMainRequest;
}

namespace NVM::Simulation {

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
     * Gets the next available request
     *
     * @return Next request from the trace. TODO: make NONE type request
     */
    NVM::NVMainRequest* getRequest();

    private:
    std::unique_ptr<TraceReader> reader;
    std::queue<NVMainRequest*> nextReqs;

    /**
     * Gets the next TraceLine from the TraceReaer and translates to one or more
     * requests. Saves the requests in nextReqs.
     */
    void convertNextTraceLine();

    void convertReadLine(TraceLine line);
    void convertWriteLine(TraceLine line);
    void convertPIMLine(TraceLine line);
    void convertShiftLine(TraceLine line);
    void convertTWriteLine(TraceLine line);
};

}; // namespace NVM::Simulation