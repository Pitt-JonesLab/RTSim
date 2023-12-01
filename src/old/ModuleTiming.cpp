#include "src/old/ModuleTiming.h"

using namespace NVM;

ModuleTiming::ModuleTiming(ncycle_t act, ncycle_t pre, ncycle_t read,
                           ncycle_t write, ncycle_t power) :
    nextActivate(act),
    nextPrecharge(pre),
    nextRead(read),
    nextWrite(write),
    nextPowerDown(power) {}

bool ModuleTiming::isIssuable(NVMainRequest* req, ncycle_t cycle) {
    switch (req->type) {
        case ACTIVATE:
        case REFRESH:
            return nextActivate <= cycle;

        case READ:
        case READ_PRECHARGE:
            return nextRead <= cycle;

        case WRITE:
        case WRITE_PRECHARGE:
            return nextWrite <= cycle;

        case PRECHARGE:
        case PRECHARGE_ALL:
            return nextPrecharge <= cycle;

        case POWERDOWN_PDA:
        case POWERDOWN_PDPF:
        case POWERDOWN_PDPS:
            return nextPowerDown <= cycle;

        case SHIFT:
        case POWERUP:
        case ROWCLONE_SRC:
        case ROWCLONE_DEST:
            return true;

        default:
            return false;
    }

    return false;
}