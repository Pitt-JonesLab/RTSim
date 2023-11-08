#pragma once

#include "Simulation/TraceReader.h"

#include <fstream>

namespace NVM::Simulation {

class FileTraceReader : public TraceReader {
    public:
    FileTraceReader(std::string traceFilename);

    private:
    std::ifstream traceFile;
};

} // namespace NVM::Simulation