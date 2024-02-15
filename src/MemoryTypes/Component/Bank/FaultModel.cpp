#include "MemoryTypes/Component/Bank/FaultModel.h"

#include "Utils/ConfigParser.h"

#include <cstdlib>

using namespace NVM::ComponentType;
using namespace NVM::Memory;

NVM::ComponentType::FaultModel::FaultModel() : numUncorrectable(0) {
    ConfigParser::registerValue("PIMFaultRate", 0.0, &faultRate);
    ConfigParser::registerValue("NumCorrectableFaults", 0, &numCorrectable);
    ConfigParser::registerValue("WordSize", 64, &wordSize);
}

bool testForFault(double faultRate) {
    double randTest = ((double) rand() / (RAND_MAX));
    return (randTest < faultRate);
}

bool NVM::ComponentType::FaultModel::check() {
    int numFaults = 0;
    for (int i = 0; i < wordSize; i++) {
        if (testForFault(faultRate)) numFaults++;
    }
    if (numFaults > numCorrectable) numUncorrectable++;
    return numFaults > 0 && numFaults <= numCorrectable;
}

int NVM::ComponentType::FaultModel::getUncorrectableFaults() const {
    return numUncorrectable;
}

void NVM::ComponentType::FaultModel::resetUncorrectables() {
    numUncorrectable = 0;
}
