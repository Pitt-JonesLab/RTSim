#pragma once

#include "Logging/LogStream.h"

#include <iostream>

namespace NVM::Logging {

/**
 * Global LogStream for the simulation
 */
LogStream& log();

/**
 * Changes the log output to the given filename
 *
 * If the file cannot be opened, logs an error and keeps the last output stream
 */
void setLogFile(std::string filename);

/**
 * Changes the log output to the given ostream
 */
void setLogOutput(std::ostream& output);

/**
 * Sets the global log level
 */
void setLogLevel(LogLevel level);

} // namespace NVM::Logging