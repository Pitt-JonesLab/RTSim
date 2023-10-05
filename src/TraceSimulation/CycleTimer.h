#pragma once

namespace NVM::Simulation {

class CycleTimer {
    public:
    /**
     * Creates a CycleTimer
     *
     * @oaram max Max cycles for this timer. 0 for infinite cycles
     */
    CycleTimer(unsigned int max = 0);

    /**
     * Attempts to advance the timer by the given number of cycles. Returns the
     * number of cycles advanced. If the return value is less than the given
     * parameter, the maximum number of cycles have been reached.
     *
     * @param cycles Number of cycles to advance
     *
     * @return Number of cycles successfully advanced
     */
    unsigned int cycle(unsigned int cycles);

    /**
     * Returns current cycle
     *
     * @return Current cycle
     */
    unsigned int getCurrentCycle() const;

    /**
     * Returns max cycle. 0 means no max.
     *
     * @return Max cycle
     */
    unsigned int getMaxCycle() const;

    private:
    // Current cycle for the timer
    unsigned int currentCycle;

    // Max cycle, 0 means no maximum
    unsigned int maxCycles;
};

} // namespace NVM::Simulation