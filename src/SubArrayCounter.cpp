#include "src/SubArrayCounter.h"

using namespace NVM;

SubArrayCounter::SubArrayCounter() :
    numRanks(0),
    numBanks(0),
    numSubArrays(0) {}

SubArrayCounter::SubArrayCounter(Params* p, Config* config) :
    numRanks(p->RANKS),
    numBanks(p->BANKS) {
    if (config->KeyExists("MATHeight")) numSubArrays = p->ROWS / p->MATHeight;
    else numSubArrays = 1;

    counters = std::vector<ncounter_t>(numRanks * numBanks * numSubArrays, 0);
}

ncounter_t& SubArrayCounter::operator[](NVMainRequest* req) {
    return (*this)[req->address];
}

size_t SubArrayCounter::getAddressIndex(NVMAddress address) const {
    ncounter_t rank, bank, subarray;

    address.GetTranslatedAddress(NULL, NULL, &bank, &rank, NULL, &subarray);

    return rank * numBanks * numSubArrays + bank * numSubArrays + subarray;
}

ncounter_t& SubArrayCounter::operator[](NVMAddress address) {
    return counters[getAddressIndex(address)];
}

ncounter_t SubArrayCounter::operator[](NVMainRequest* req) const {
    return (*this)[req->address];
}

ncounter_t SubArrayCounter::operator[](NVMAddress address) const {
    return counters[getAddressIndex(address)];
}

void SubArrayCounter::increment(NVMainRequest* req) { increment(req->address); }

void SubArrayCounter::increment(NVMAddress address) {
    counters[getAddressIndex(address)]++;
}

void SubArrayCounter::clear(NVMainRequest* req) { clear(req->address); }

void SubArrayCounter::clear(NVMAddress address) {
    counters[getAddressIndex(address)] = 0;
}