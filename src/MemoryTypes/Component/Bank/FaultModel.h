#pragma once

namespace NVM::ComponentType {

class FaultModel {
    public:
    FaultModel();

    /*
     *   Returns true if a correctable fault occurred in the row, false
     * otherwise
     */
    bool check();

    int getUncorrectableFaults() const;

    /*
     *   Resets uncorrectable fault counter to 0
     */
    void resetUncorrectables();

    private:
    double faultRate;
    int numCorrectable, wordSize, numUncorrectable;
};

} // namespace NVM::ComponentType