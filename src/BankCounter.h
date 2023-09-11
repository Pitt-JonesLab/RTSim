#pragma once

#include "include/NVMAddress.h"
#include "include/NVMainRequest.h"
#include "src/Params.h"

#include <vector>

namespace NVM {

class BankCounter {
    public:
    BankCounter();
    BankCounter(Params* p, ncounter_t initial = 0);

    ncounter_t& operator[](NVMainRequest* req);
    ncounter_t& operator[](NVMAddress address);

    ncounter_t operator[](NVMainRequest* req) const;
    ncounter_t operator[](NVMAddress address) const;

    void increment(NVMainRequest* req);
    void increment(NVMAddress address);

    void clear(NVMainRequest* req, ncounter_t reset = 0);
    void clear(NVMAddress address, ncounter_t reset = 0);

    /*
     *   Clears all values in an entire bank
     *
     *   @param rank Rank of the bank to clear
     *   @param bank Index of the bank to clear
     */
    void clear(size_t rank, size_t bank, ncounter_t reset = 0);

    private:
    size_t numRanks, numBanks;

    std::vector<ncounter_t> counters;

    size_t getAddressIndex(NVMAddress address) const;
};

} // namespace NVM