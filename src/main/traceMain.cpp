/*******************************************************************************
 * Copyright (c) 2012-2014, The Microsystems Design Labratory (MDL)
 * Department of Computer Science and Engineering, The Pennsylvania State
 *University All rights reserved.
 *
 * This source code is part of NVMain - A cycle accurate timing, bit accurate
 * energy simulator for both volatile (e.g., DRAM) and non-volatile memory
 * (e.g., PCRAM). The source code is free and you can redistribute and/or
 * modify it by providing that the following conditions are met:
 *
 *  1) Redistributions of source code must retain the above copyright notice,
 *     this list of conditions and the following disclaimer.
 *
 *  2) Redistributions in binary form must reproduce the above copyright notice,
 *     this list of conditions and the following disclaimer in the documentation
 *     and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 *AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 *IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
 * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
 * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 * Author list:
 *   Matt Poremba    ( Email: mrp5060 at psu dot edu
 *                     Website: http://www.cse.psu.edu/~poremba/ )
 *******************************************************************************/

#include "Decoders/DecoderFactory.h"
#include "Endurance/EnduranceDistributionFactory.h"
#include "include/NVMHelpers.h"
#include "Interconnect/InterconnectFactory.h"
#include "MemControl/MemoryControllerFactory.h"
#include "NVM/nvmain.h"
#include "SimInterface/NullInterface/NullInterface.h"
#include "src/AddressTranslator.h"
#include "src/Config.h"
#include "src/EventQueue.h"
#include "src/Interconnect.h"
#include "src/main/RTSystem.h"
#include "src/MemoryController.h"
#include "src/Simulation/FileTraceReader.h"
#include "src/Simulation/TraceSimulator.h"
#include "src/TranslationMethod.h"
#include "Utils/HookFactory.h"

#include <cmath>
#include <fstream>
#include <memory>
#include <sstream>
#include <stdlib.h>

using namespace NVM;
using namespace NVM::Simulation;
using namespace NVM::Main;

ncycle_t getMaxCycles(Config* config, char* arg) {
    ncycle_t maxCycles = 0;

    if (arg != nullptr) maxCycles = atoi(arg);

    return (uint64_t) ceil(((double) (config->GetValue("CPUFreq")) /
                            (double) (config->GetValue("CLK"))) *
                           maxCycles);
}

Config* createConfig(int argc, char* argv[]) {
    Config* config = new Config();
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

    // Build dependencies
    Config* config = createConfig(argc, argv);
    ncycle_t simulateCycles =
        getMaxCycles(config, (argc > 3) ? argv[3] : nullptr);

    // Build RTSystem
    auto memory = std::make_unique<RTSystem>(config);

    // Build TraceReader
    auto reader = std::make_unique<FileTraceReader>(argv[2]);

    // Build simulator and run
    TraceSimulator simulator(std::move(reader), std::move(memory),
                             simulateCycles);
    simulator.run();
    simulator.printStats(std::cout);

    return 0;
}