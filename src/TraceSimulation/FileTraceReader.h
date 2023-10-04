#pragma once

#include "src/TraceSimulation/TraceReader.h"

#include <fstream>

namespace NVM::Simulation {

class FileTraceReader : public TraceReader {
    public:
    FileTraceReader(std::string traceFilename);

    TraceLine getLine();

    private:
    std::ifstream traceFile;
    int traceVersion;
};

} // namespace NVM::Simulation