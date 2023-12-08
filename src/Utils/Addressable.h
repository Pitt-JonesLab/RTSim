#pragma once

#include "Utils/Address.h"

#include <memory>
#include <vector>

namespace NVM {

/*
 *   An Addressable is a nested structure
 */
template<typename T> class Addressable {
    public:
    Addressable(unsigned int numChildren, unsigned int lsb, unsigned int msb) :
        lsb(lsb),
        msb(msb) {
        for (int i = 0; i < numChildren; i++) {
            children.emplace_back(new T());
        }
    }

    const T& decode(Address address) const {
        auto index = address.getBitRange(msb, lsb);
        return *children[index];
    }

    T& decode(Address address) {
        auto index = address.getBitRange(msb, lsb);
        return *children[index];
    }

    private:
    std::vector<std::unique_ptr<T>> children;

    unsigned int msb, lsb;
};

} // namespace NVM