#pragma once

#include "include/NVMainRequest.h"

namespace NVM {

class ModuleTiming {
    public:
    ModuleTiming(ncycle_t act, ncycle_t pre, ncycle_t read, ncycle_t write,
                 ncycle_t power);

    bool isIssuable(NVMainRequest* req, ncycle_t cycle);

    private:
    ncycle_t lastActivate;
    ncycle_t nextActivate;
    ncycle_t nextPrecharge;
    ncycle_t nextRead;
    ncycle_t nextWrite;
    ncycle_t nextPowerDown;
};

} // namespace NVM