#include "Modeling/Bank/SimpleBank.h"

#include "Modeling/Decoder.h"
#include "Modeling/SubArray.h"

using namespace NVM::Modeling;

SimpleBank::SimpleBank() {}

void SimpleBank::addSubArray(std::unique_ptr<SubArray> subArray) {
    subArrays.emplace_back(std::move(subArray));
}

const SubArray& SimpleBank::decodeSubArray(Address address) const {
    return *subArrays[0];
}

SubArray& SimpleBank::decodeSubArray(Address address) { return *subArrays[0]; }

RowNumber SimpleBank::getOpenRow(Address address) const {
    return decodeSubArray(address).getOpenRow();
}

bool SimpleBank::isOpen(Address address) const {
    return decodeSubArray(address).isOpen();
}

void SimpleBank::closeRow(Address address) {
    decodeSubArray(address).closeRow();
}

void SimpleBank::openRow(Address address) {
    auto row = decodeSymbol(AddressSymbol::ROW, address);
    decodeSubArray(address).openRow(row);
}