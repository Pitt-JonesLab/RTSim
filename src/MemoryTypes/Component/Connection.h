#pragma once

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
    Connection() : valid(false), currentCommand(CommandType::Opcode::NO_OP){};

    /*
     *   Puts a command on the Connection.
     *   If there is already a command present, throws an exception.
     */
    void issue(CommandType cmd) {
        if (valid) throw std::runtime_error("Connection contention!");

        currentCommand = cmd;
        valid = true;
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
    void cycle() { valid = false; }

    private:
    CommandType currentCommand;
    bool valid;
};

} // namespace NVM::ComponentType