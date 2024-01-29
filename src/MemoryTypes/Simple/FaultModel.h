#pragma once

namespace NVM::Memory {

class FaultModel {
    public:
    FaultModel(double faultRate, int wordSize, int numCorrectable);

    /*
     *   Returns true if a correctable fault occurred, false otherwise
     */
    bool check();

    int getUncorrectableFaults() const;

    private:
    double faultRate;
    int numCorrectable, wordSize, numUncorrectable;
};

} // namespace NVM::Memory