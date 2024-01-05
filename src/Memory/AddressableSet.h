#pragma once

#include "Utils/Address.h"

#include <memory>
#include <vector>

namespace NVM::Memory {

template<typename T> class AddressableSet {
    public:
    template<typename... Args>
    AddressableSet(unsigned int msb, unsigned int lsb, size_t numItems,
                   Args... args) {
        for (int i = 0; i < numItems; i++) {
            items.emplace_back(new T(args...));
        }
    }

    T& at(Address address) {
        auto index = address.getBitRange(msb, lsb);
        return *items[index];
    }

    private:
    std::vector<std::unique_ptr<T>> items;
    unsigned int msb, lsb;
};

} // namespace NVM::Memory