#include "Utils/RowData.h"

using namespace NVM;

RowData::RowData() : data({0}) {}

void RowData::setByte(size_t index, uint8_t byte) { data[index] = byte; }

uint8_t RowData::getByte(size_t index) const { return data[index]; }