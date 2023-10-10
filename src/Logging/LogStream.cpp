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

void LogStream::addStatTag(const std::string& tag) { statTags.push_back(tag); }

void LogStream::popStatTag() {
    if (!statTags.empty()) statTags.pop_back();
}

void LogStream::printTags() {
    if (statTags.empty()) return;
    for (auto tag = statTags.begin(); tag != (statTags.end() - 1); tag++) {
        log(*tag);
        log('.');
    }
    log(statTags.back());
}

LogStream& NVM::Logging::useTags(LogStream& stream) {
    stream.printTags();
    return stream;
}

LogStream& NVM::Logging::operator<<(LogStream& stream,
                                    LogStream& (*func)(LogStream&) ) {
    return func(stream);
}