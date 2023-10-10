#include "Logging/Logging.h"

#include <fstream>

using namespace NVM::Logging;

LogStream logStream;

LogStream& NVM::Logging::log() { return logStream; }

void NVM::Logging::setLogFile(std::string filename) {
    std::ofstream logFile(filename);

    if (!logFile.good()) {
        logStream << LogLevel::ERROR << "Could not set log file: " << filename
                  << '\n';
        return;
    }
    logStream = LogStream(logFile);
}

void NVM::Logging::setLogLevel(LogLevel level) {
    logStream.setGlobalLevel(level);
}