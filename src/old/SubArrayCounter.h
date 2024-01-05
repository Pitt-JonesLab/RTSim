#pragma once

#include "src/old/NVMAddress.h"
#include "src/old/NVMainRequest.h"
#include "src/old/Params.h"

#include <vector>

namespace NVM {

class SubArrayCounter {
    public:
    SubArrayCounter();
    SubArrayCounter(Params* p, Config* config, ncounter_t initial = 0);

    ncounter_t& operator[](NVMainRequest* req);
    ncounter_t& operator[](NVMAddress address);

    ncounter_t operator[](NVMainRequest* req) const;
    ncounter_t operator[](NVMAddress address) const;

    void increment(NVMainRequest* req);
    void increment(NVMAddress address);

    void clear(NVMainRequest* req, ncounter_t reset = 0);
    void clear(NVMAddress address, ncounter_t reset = 0);

    std::vector<ncounter_t>::iterator begin();
    std::vector<ncounter_t>::iterator begin(size_t rankNum, size_t bankNum);

    std::vector<ncounter_t>::iterator end();
    std::vector<ncounter_t>::iterator end(size_t rankNum, size_t bankNum);

    /*
     *   Clears all values in an entire bank
     *
     *   @param rank Rank of the bank to clear
     *   @param bank Index of the bank to clear
     */
    void clear(size_t rank, size_t bank, ncounter_t reset = 0);

    private:
    size_t numRanks, numBanks, numSubArrays;

    std::vector<ncounter_t> counters;

    size_t getAddressIndex(NVMAddress address) const;
};

} // namespace NVM