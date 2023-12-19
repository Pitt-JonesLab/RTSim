#include "MemoryTypes/Simple/FaultModel.h"

#include <cstdlib>

using namespace NVM::Memory;

FaultModel::FaultModel(double faultRate) : faultRate(faultRate) {}

bool FaultModel::check() const {
    double randTest = ((double) rand() / (RAND_MAX));
    if (randTest < faultRate) return true;
    return false;
}