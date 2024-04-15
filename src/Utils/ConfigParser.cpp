#include "Utils/ConfigParser.h"

using namespace NVM::Memory;

bool ConfigParser::hasConfig = false;
NVM::Simulation::Config ConfigParser::config;
std::vector<std::function<void(const NVM::Simulation::Config&)>>
    ConfigParser::parsers;