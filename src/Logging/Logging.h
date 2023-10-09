#pragma once

#include "Logging/LogStream.h"

namespace NVM::Logging {

/**
 * Global LogStream for the simulation
 */
LogStream log;

/**
 * Changes the log output to the given filename
 *
 * If the file cannot be opened, logs an error and keeps the last output stream
 */
void setLogFile(std::string filename);

/**
 * Sets the global log level
 */
void setLogLevel(LogLevel level);

} // namespace NVM::Logging