#pragma once

#include <iostream>

namespace NVM::Logging {

/**
 * In order from lowest to highest verbosity
 *
 * ERROR: Messages to send before throwing an exception
 * STAT: Statistical remarks or results from simulation
 * EVENT: Events that occur during simulation. (Rank received request, etc.)
 * DEBUG: Temporary messages for debugging new features
 */
enum class LogLevel { ERROR, STAT, EVENT, DEBUG };

class LogStream {
    public:
    /**
     * Creates a LogStream that writes to cout
     */
    LogStream();

    /**
     * Creates a LogStream
     */
    LogStream(std::ostream&);

    /**
     * Returns the current level of the LogStream
     */
    LogLevel getLevel() const;

    /**
     * Sets the level of the LogStream
     */
    void setLevel(LogLevel level);

    void setGlobalLevel(LogLevel level);

    /**
     * Logs a message at the current level
     */
    template<typename T> void log(T message) {
        if (level <= globalLevel) (*output) << message;
    }

    private:
    LogLevel level, globalLevel;

    std::ostream* output;
};

/**
 * Logs the given message at the current level of the LogStream
 */
template<typename T> LogStream& operator<<(LogStream& log, T message) {
    log.log(message);
    return log;
}

/**
 * Sets the level of the LogStream
 */
LogStream& operator<<(LogStream&, LogLevel level);

} // namespace NVM::Logging