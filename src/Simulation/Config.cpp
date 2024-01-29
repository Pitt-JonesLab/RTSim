#include "Config.h"

#include "Logging/Logging.h"

#include <fstream>
#include <iostream>

using namespace NVM::Simulation;
using namespace NVM::Logging;

Config::Config() {}

template<typename T> T Config::get(std::string key) const {
    return std::any_cast<T>(values.at(key));
}

template<typename T> void Config::set(std::string key, T value) {
    values[key] = value;
}

template int Config::get(std::string key) const;

template<> unsigned int Config::get(std::string key) const {
    return get<int>(key);
}

template bool Config::get(std::string key) const;
template std::string Config::get(std::string key) const;
template double Config::get(std::string key) const;

template void Config::set(std::string key, int);
template void Config::set(std::string key, double);
template void Config::set(std::string key, bool);
template void Config::set(std::string key, std::string);

Config NVM::Simulation::readConfig(std::string filename) {
    std::ifstream configFile(filename);

    if (!configFile.is_open()) {
        throw std::runtime_error("Could not open config file " + filename);
    }

    std::cout << "Reading config info from " << filename << '\n';

    return readConfig(configFile);
}

void setKey(Config& conf, std::string key, std::string val) {
    if (val.find_first_not_of("0123456789.") != std::string::npos) {
        conf.set(key, val);
        if (val == "true") conf.set(key, true);
        if (val == "false") conf.set(key, false);
        return;
    }

    try {
        conf.set(key, std::stoi(val));
    } catch (...) {
    }

    if (val.find(".") != std::string::npos) {
        try {
            conf.set(key, std::stod(val));
        } catch (...) {
        }
    }
}

Config NVM::Simulation::readConfig(std::istream& input) {
    Config conf;

    while (!input.eof()) {
        std::string line;
        getline(input, line);

        if (line.find(";") != std::string::npos)
            line = line.substr(0, line.find(";"));
        if (line.find_last_not_of(" \t\r\n") == std::string::npos) continue;

        std::string key = line.substr(0, line.find(" "));
        line = line.substr(line.find(" ") + 1);

        if (line.find_last_not_of(" \t\r\n") < line.size() - 1)
            line = line.substr(0, line.find_last_not_of(" \t\r\n") + 1);

        log() << LogLevel::DEBUG << key << " = " << line << '\n';
        setKey(conf, key, line);
    }

    return conf;
}