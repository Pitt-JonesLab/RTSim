#include "Logging/Logging.h"
#include "Memory/Decoder.h"
#include "Memory/MemoryFactory.h"
#include "Simulation/Config.h"
#include "Simulation/TraceReader/FileTraceReader.h"
#include "Simulation/TraceSimulator.h"
#include "src/old/Config.h"
#include "src/old/NVMTypes.h"

#include <cmath>
#include <fstream>
#include <iostream>
#include <memory>
#include <sstream>
#include <stdlib.h>

using namespace NVM;
using namespace NVM::Memory;
using namespace NVM::Simulation;
using namespace NVM::Logging;

ncycle_t getMaxCycles(NVM::Config* config, char* arg) {
    ncycle_t maxCycles = 0;

    if (arg != nullptr) maxCycles = atoi(arg);

    return (uint64_t) ceil(((double) (config->GetValue("CPUFreq")) /
                            (double) (config->GetValue("CLK"))) *
                           maxCycles);
}

NVM::Config* createConfig(int argc, char* argv[]) {
    NVM::Config* config = new NVM::Config();
    config->Read(argv[1]);
    if (argc > 4) {
        for (int curArg = 4; curArg < argc; ++curArg) {
            std::string clParam, clValue, clPair;

            clPair = argv[curArg];
            clParam = clPair.substr(0, clPair.find_first_of("="));
            clValue =
                clPair.substr(clPair.find_first_of("=") + 1, std::string::npos);

            std::cout << "Overriding " << clParam << " with '" << clValue << "'"
                      << std::endl;

            config->SetValue(clParam, clValue);
        }
    }
    return config;
}

void setAddressScheme(const NVM::Simulation::Config& conf) {
    Decoder::ComponentCounts counts;

    counts.rows = conf.get<int>("DBCS");
    counts.cols = conf.get<int>("DOMAINS");
    counts.ranks = conf.get<int>("RANKS");
    counts.banks = conf.get<int>("BANKS");
    counts.channels = conf.get<int>("CHANNELS");

    Decoder::setScheme(conf.get<std::string>("AddressMappingScheme"), counts);
}

int main(int argc, char* argv[]) {
    // Check for correct args
    if (argc < 4) {
        std::cout
            << "Usage: nvmain CONFIG_FILE TRACE_FILE CYCLES [PARAM=value ...]"
            << std::endl;
        return 1;
    }

    // Print args
    std::cout << "NVMain command line is:" << std::endl;
    for (int curArg = 0; curArg < argc; ++curArg) {
        std::cout << argv[curArg] << " ";
    }
    std::cout << std::endl << std::endl;

    // Set up log
    Logging::setLogLevel(LogLevel::DEBUG);

    // Build dependencies
    NVM::Config* config = createConfig(argc, argv);
    ncycle_t simulateCycles =
        getMaxCycles(config, (argc > 3) ? argv[3] : nullptr);

    NVM::Simulation::Config conf = readConfig(argv[1]);
    setAddressScheme(conf);

    // Build RTSystem
    auto memory = makeSimpleSystem(conf);

    // Build TraceReader
    auto reader = std::make_unique<FileTraceReader>(argv[2]);

    // Build simulator and run
    TraceSimulator simulator(std::move(reader), std::move(memory),
                             simulateCycles);
    simulator.run();
    simulator.printStats(std::cout);

    return 0;
}