#pragma once

namespace NVM::ComponentType {

template<typename CommandType> class TimingRules {
    public:
    virtual ~TimingRules() {}

    /*
     *   Signals a command being issued in the current cycle. Allows the
     * implementation to maintain a history of sent commands.
     */
    virtual void issue(CommandType cmd) = 0;

    /*
     *   Returns true if the provided command can be sent this cycle, false
     * otherwise.
     */
    virtual bool validate(CommandType cmd) = 0;

    /*
     *   Signals a cycle event so TimingRules implementations can maintain an
     * accurate history.
     */
    virtual void cycle() = 0;
};

} // namespace NVM::ComponentType