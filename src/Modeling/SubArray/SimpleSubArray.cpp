#include "Modeling/SubArray/SimpleSubArray.h"

#include <stdexcept>

using namespace NVM::Modeling;

SimpleSubArray::SimpleSubArray() : open(false), row(0) {}

RowNumber SimpleSubArray::getOpenRow() const {
    if (!open) throw std::runtime_error("SubArray is not open!");
    return row;
}

bool SimpleSubArray::isOpen() const { return open; }

void SimpleSubArray::closeRow() { open = false; }

void SimpleSubArray::openRow(RowNumber r) {
    if (open) throw std::runtime_error("SubArray is already open!");
    open = true;
    row = r;
}