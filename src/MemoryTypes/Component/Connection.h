#pragma once

#include "MemoryTypes/Component/TimingRules.h"

#include <memory>
#include <stdexcept>

namespace NVM::ComponentType {

/*
 *   Connections allow Components to send commands to each other.
 *   The CommandType is a template parameter to allow different instruction sets
 * for different Connections. For example, the memory controller may send a
 * different set of commands to the bus than a bank may send to its subarrays.
 *
 *   CommandType must provide an enum class Opcode which contains at least NO_OP
 * and a constructor that takes an Opcode.
 */
template<typename CommandType> class Connection {
    public:
    Connection() :
        valid(false),
        nextValid(false),
        currentCommand(CommandType::Opcode::NO_OP),
        nextCommand(CommandType::Opcode::NO_OP),
        timingRules(nullptr){};

    /*
     *   Puts a command on the Connection.
     *   If there is already a command present, throws an exception.
     */
    void issue(CommandType cmd) {
        if (nextValid) throw std::runtime_error("Connection contention!");
        if (timingRules) timingRules->issue(cmd);
        nextCommand = cmd;
        nextValid = true;
    }

    /*
     *   Returns the current command on the Connection.
     *   If there is no command, returns a NO_OP command.
     */
    CommandType receive() const {
        if (!valid) return CommandType(CommandType::Opcode::NO_OP);
        return currentCommand;
    }

    /*
     *   Resets the Connection command.
     */
    void cycle() {
        valid = nextValid;
        currentCommand = nextCommand;
        nextValid = false;
        if (timingRules) timingRules->cycle();
    }

    /*
     *   Sets the TimingRules object for this Connection.
     */
    void setTimingRules(std::unique_ptr<TimingRules<CommandType>>&& rules) {
        timingRules = std::move(rules);
    }

    bool validateTiming(CommandType cmd) {
        if (!timingRules) return true;
        return timingRules->validate(cmd);
    }

    private:
    CommandType currentCommand, nextCommand;
    bool valid, nextValid;
    std::unique_ptr<TimingRules<CommandType>> timingRules;
};

} // namespace NVM::ComponentType