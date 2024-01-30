#pragma once

#include "Utils/Address.h"
#include "Utils/RowData.h"

#include <stdexcept>

namespace NVM::State {

/*
 *   Interface for any StateMemory device. Contains all possible commands.
 */
class Commandable {
    public:
    /*
     *   Performs a READ command.
     */
    virtual bool read(const Address& address, const RowData& data) = 0;

    /*
     *   Performs a WRITE command.
     */
    virtual bool write(const Address& address, const RowData& data) = 0;

    /*
     *   Performs an ACTIVATE command.
     */
    virtual bool activate(const Address& address) = 0;

    /*
     *   Performs a PRECHARGE command.
     */
    virtual bool precharge(const Address& address) = 0;

    /*
     *   Performs a REFRESH command.
     */
    virtual bool refresh() = 0;

    /*
     *   Default function for denying a command.
     */
    template<typename... ArgTypes> bool deny(ArgTypes... args) { return false; }

    /*
     *   Default function for throwing an exception when receiving a command.
     */
    template<typename... ArgTypes> bool except(ArgTypes... args) {
        throw std::runtime_error("Bad command!");
    }
};

} // namespace NVM::State