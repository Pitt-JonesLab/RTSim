#include "MemoryTypes/Simple/FaultModel.h"

#include <cstdlib>

using namespace NVM::Memory;

FaultModel::FaultModel(double faultRate, int wordSize, int numCorrectable) :
    faultRate(faultRate),
    numCorrectable(numCorrectable),
    numUncorrectable(0),
    wordSize(wordSize) {}

bool testFault(double faultRate) {
    double randTest = ((double) rand() / (RAND_MAX));
    return (randTest < faultRate);
}

bool FaultModel::check() {
    int numFaults = 0;
    for (int i = 0; i < wordSize; i++) {
        if (testFault(faultRate)) numFaults++;
    }
    if (numFaults > numCorrectable) numUncorrectable++;
    return numFaults > 0 && numFaults <= numCorrectable;
}

int NVM::Memory::FaultModel::getUncorrectableFaults() const {
    return numUncorrectable;
}
