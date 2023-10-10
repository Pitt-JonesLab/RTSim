#include "Logging/LogStream.h"

#include <iostream>

using namespace NVM::Logging;

LogStream::LogStream() : globalLevel(LogLevel::EVENT), output(&std::cout) {}

LogStream::LogStream(std::ostream& output) :
    globalLevel(LogLevel::EVENT),
    output(&output) {}

LogLevel LogStream::getLevel() const { return level; }

void LogStream::setLevel(LogLevel l) { level = l; }

void LogStream::setGlobalLevel(LogLevel l) { globalLevel = l; }

LogStream& NVM::Logging::operator<<(LogStream& log, LogLevel level) {
    log.setLevel(level);
    return log;
}