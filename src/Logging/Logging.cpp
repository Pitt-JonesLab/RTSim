#include "Logging/Logging.h"

#include <fstream>

using namespace NVM::Logging;

void setLogFile(std::string filename) {
    std::ofstream logFile(filename);

    if (!logFile.good()) {
        log << LogLevel::ERROR << "Could not set log file: " << filename
            << '\n';
        return;
    }
    log = LogStream(logFile);
}

void setLogLevel(LogLevel level) { log.setGlobalLevel(level); }