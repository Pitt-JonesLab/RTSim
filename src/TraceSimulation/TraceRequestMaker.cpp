#include "src/TraceSimulation/TraceRequestMaker.h"

using namespace NVM::Simulation;

TraceRequestMaker::TraceRequestMaker(std::unique_ptr<TraceReader> reader) {}

TraceRequestMaker::TraceRequestMaker(const TraceRequestMaker& rhs) {}

const TraceRequestMaker&
TraceRequestMaker::operator=(const TraceRequestMaker& rhs) {
    return *this;
}

NVM::NVMainRequest* TraceRequestMaker::getRequest() { return nullptr; }

void TraceRequestMaker::convertNextTraceLine() {}