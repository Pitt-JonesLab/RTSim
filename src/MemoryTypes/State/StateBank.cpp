#include "MemoryTypes/State/StateBank.h"

using namespace NVM::State;
using NVM::Address;
using NVM::RowData;

StateBank::StateBank() {}

/*
 *   Performs a READ command.
 */
bool StateBank::read(const Address& address, const RowData& data) {
    return true;
}

/*
 *   Performs a WRITE command.
 */
bool StateBank::write(const Address& address, const RowData& data) {
    return true;
}

/*
 *   Performs an ACTIVATE command.
 */
bool StateBank::activate(const Address& address) { return true; }

/*
 *   Performs a PRECHARGE command.
 */
bool StateBank::precharge(const Address& address) { return true; }

/*
 *   Performs a REFRESH command.
 */
bool StateBank::refresh() { return true; }