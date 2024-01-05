#pragma once

#include <string>
#include <map>
#include <any>

namespace NVM::Simulation{
/*
*   Holds all configuration information for the simulation
*/
class Config {
    public:
    Config();

    template <typename T>
    T get(std::string key) const;

    template <typename T>
    void set(std::string key, T value);

    private:
    std::map<std::string, std::any> values;
};

Config readConfig(std::string filename);

Config readConfig(std::istream& input);

}