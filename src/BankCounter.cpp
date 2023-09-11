#include "src/BankCounter.h"

using namespace NVM;

BankCounter::BankCounter() {}

BankCounter::BankCounter(Params* p, ncounter_t initial) {
    numRanks = p->RANKS;
    numBanks = p->BANKS;

    counters = std::vector<ncounter_t>(numRanks * numBanks);
}

ncounter_t& BankCounter::operator[](NVMainRequest* req) {
    return (*this)[req->address];
}

ncounter_t& BankCounter::operator[](NVMAddress address) {
    return counters[getAddressIndex(address)];
}

ncounter_t BankCounter::operator[](NVMainRequest* req) const {
    return (*this)[req->address];
}

ncounter_t BankCounter::operator[](NVMAddress address) const {
    return counters[getAddressIndex(address)];
}

void BankCounter::increment(NVMainRequest* req) { increment(req->address); }

void BankCounter::increment(NVMAddress address) {
    counters[getAddressIndex(address)]++;
}

void BankCounter::clear(NVMainRequest* req, ncounter_t reset) {
    clear(req->address, reset);
}

void BankCounter::clear(NVMAddress address, ncounter_t reset) {
    counters[getAddressIndex(address)] = reset;
}

void BankCounter::clear(size_t rank, size_t bank, ncounter_t reset) {
    counters[rank * numBanks + bank] = reset;
}

size_t BankCounter::getAddressIndex(NVMAddress address) const {
    return address.GetRank() * numBanks + address.GetBank();
}