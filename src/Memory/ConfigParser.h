#pragma once

#include "Logging/Logging.h"
#include "Simulation/Config.h"

#include <functional>
#include <string>
#include <vector>

namespace NVM::Memory {

class ConfigParser {
    public:
    ConfigParser() {}

    template<typename T>
    void registerValue(std::string key, T defaultValue, T* destination) {
        using namespace NVM::Logging;

        log() << LogLevel::DEBUG << "Registering config value " << key
              << ", default is " << defaultValue << '\n';

        parsers.push_back([key, defaultValue,
                           destination](const NVM::Simulation::Config& conf) {
            try {
                *destination = conf.get<T>(key);
                log() << LogLevel::STAT << "Read config value " << key << " as "
                      << *destination << '\n';
            } catch (...) {
                log() << LogLevel::STAT << "Using default config value for "
                      << key << " (" << *destination << ")\n";
                *destination = defaultValue;
            }
        });
    }

    void parse(const NVM::Simulation::Config& conf) {
        for (auto p : parsers) p(conf);
    }

    private:
    std::vector<std::function<void(const NVM::Simulation::Config&)>> parsers;
};

} // namespace NVM::Memory