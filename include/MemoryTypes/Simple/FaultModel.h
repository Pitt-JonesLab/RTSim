#pragma once

namespace NVM::Memory {

class FaultModel {
    public:
    FaultModel(double faultRate);

    bool check() const;

    private:
    double faultRate;
};

} // namespace NVM::Memory