#pragma once

#include "include/NVMAddress.h"
#include "include/NVMainRequest.h"
#include "src/Params.h"

#include <vector>

namespace NVM {

class SubArrayCounter {
    public:
    SubArrayCounter();
    SubArrayCounter(Params* p, Config* config);

    ncounter_t& operator[](NVMainRequest* req);
    ncounter_t& operator[](NVMAddress address);

    ncounter_t operator[](NVMainRequest* req) const;
    ncounter_t operator[](NVMAddress address) const;

    void increment(NVMainRequest* req);
    void increment(NVMAddress address);

    void clear(NVMainRequest* req);
    void clear(NVMAddress address);

    private:
    size_t numRanks, numBanks, numSubArrays;

    std::vector<ncounter_t> counters;

    size_t getAddressIndex(NVMAddress address) const;
};

} // namespace NVM