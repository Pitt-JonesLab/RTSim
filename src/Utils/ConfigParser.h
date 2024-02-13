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
    static void registerValue(std::string key, T defaultValue, T* destination) {
        using namespace NVM::Logging;

        log() << LogLevel::DEBUG << "Registering config value " << key
              << ", default is " << defaultValue << '\n';

        auto parseFunc = [key, defaultValue,
                          destination](const NVM::Simulation::Config& conf) {
            try {
                *destination = conf.get<T>(key);
                log() << LogLevel::STAT << "Read config value " << key << " as "
                      << *destination << '\n';
            } catch (...) {
                log() << LogLevel::STAT << "Using default config value for "
                      << key << " (" << defaultValue << ")\n";
                *destination = defaultValue;
            }
        };

        if (hasConfig) {
            parseFunc(config);
        } else {
            parsers.push_back(parseFunc);
        }
    }

    static void setConfig(const NVM::Simulation::Config& conf) {
        config = conf;
        for (auto p : parsers) p(conf);
        hasConfig = true;
    }

    private:
    static std::vector<std::function<void(const NVM::Simulation::Config&)>>
        parsers;

    static NVM::Simulation::Config config;
    static bool hasConfig;
};

} // namespace NVM::Memory