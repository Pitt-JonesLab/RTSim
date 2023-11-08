#include "Simulation/FileTraceReader.h"

using namespace NVM::Simulation;

FileTraceReader::FileTraceReader(std::string traceFilename) :
    traceFile(traceFilename),
    TraceReader(traceFile) {}