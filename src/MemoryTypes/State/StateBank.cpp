#include "MemoryTypes/State/StateBank.h"

using namespace NVM::State;
using NVM::Address;
using NVM::RowData;

StateBank::StateBank() {}

/*
 *   Performs a READ command.
 */
bool read(const Address& address, const RowData& data) { return true; }

/*
 *   Performs a WRITE command.
 */
bool write(const Address& address, const RowData& data) { return true; }

/*
 *   Performs an ACTIVATE command.
 */
bool activate(const Address& address) { return true; }

/*
 *   Performs a PRECHARGE command.
 */
bool precharge(const Address& address) { return true; }

/*
 *   Performs a REFRESH command.
 */
bool refresh() { return true; }