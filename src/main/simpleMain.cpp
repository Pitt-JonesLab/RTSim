#include "Logging/Logging.h"
#include "Memory/MemoryFactory.h"
#include "MemoryTypes/Simple/Modeling/Decoder.h"
#include "Simulation/Config.h"
#include "Simulation/TraceReader/FileTraceReader.h"
#include "Simulation/TraceSimulator.h"
#include "src/old/NVMTypes.h"
#include "Utils/ConfigParser.h"

#include <iostream>
#include <memory>
#include <stdlib.h>

using namespace NVM;
using namespace NVM::Memory;
using namespace NVM::Simulation;
using namespace NVM::Logging;
using namespace NVM::Modeling;

ncycle_t getMaxCycles(char* arg) {
    if (arg != nullptr) return atoi(arg);
    return 0;
}

void setAddressScheme() {
    ComponentCounts counts;

    ConfigParser::registerValue<unsigned int>("DBCS", 64, &counts.rows);
    ConfigParser::registerValue<unsigned int>("DOMAINS", 512, &counts.cols);
    ConfigParser::registerValue<unsigned int>("RANKS", 1, &counts.ranks);
    ConfigParser::registerValue<unsigned int>("BANKS", 1, &counts.banks);
    ConfigParser::registerValue<unsigned int>("CHANNELS", 1, &counts.channels);

    std::string addressScheme;
    ConfigParser::registerValue<std::string>("AddressMappingScheme",
                                             "RK:BK:CH:R:C", &addressScheme);

    setScheme(addressScheme, counts);
}

void setLogLevel() {
    std::string logLevel;
    ConfigParser::registerValue<std::string>("LogLevel", "STAT", &logLevel);

    if (logLevel == "DEBUG") {
        log().setGlobalLevel(LogLevel::DEBUG);
    } else if (logLevel == "EVENT") {
        log().setGlobalLevel(LogLevel::EVENT);
    } else if (logLevel == "STAT") {
        log().setGlobalLevel(LogLevel::STAT);
    } else if (logLevel == "ERROR") {
        log().setGlobalLevel(LogLevel::ERROR);
    } else {
        log().setGlobalLevel(LogLevel::STAT);
    }
}

void setSeed() { srand(time(nullptr)); }

int main(int argc, char* argv[]) {
    // Check for correct args
    if (argc < 4) {
        std::cout
            << "Usage: nvmain CONFIG_FILE TRACE_FILE CYCLES [PARAM=value ...]"
            << std::endl;
        return 1;
    }

    setSeed();

    // Build dependencies
    ncycle_t simulateCycles = getMaxCycles((argc > 3) ? argv[3] : nullptr);

    NVM::Simulation::Config conf = readConfig(argv[1]);
    ConfigParser::setConfig(conf);

    for (int i = 4; i < argc; i++) {
        conf.override(argv[i]);
    }

    setLogLevel();
    setAddressScheme();

    // Build RTSystem
    std::unique_ptr<MemorySystem> memory;

    std::string memType;
    ConfigParser::registerValue<std::string>("MemType", "Simple", &memType);

    if (memType == "Simple") {
        memory = makeSimpleSystem();
    } else if (memType == "State") {
        memory = makeStateSystem();
    } else if (memType == "Component") {
        memory = makeComponentSystem();
    }

    // Build TraceReader
    auto reader = std::make_unique<FileTraceReader>(argv[2]);

    // Build simulator and run
    TraceSimulator simulator(std::move(reader), std::move(memory),
                             simulateCycles);
    simulator.run();
    simulator.printStats(std::cout);

    return 0;
}