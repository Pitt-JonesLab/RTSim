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
     * Attempts to advance the timer by the given number of cycles
     *
     * @param cycles Number of cycles to advance
     *
     * @return True if successful, false if max cycles reached
     */
    bool cycle(unsigned int cycles);

    private:
    // Current cycle for the timer
    unsigned int currentCycle;

    // Max cycle, 0 means no maximum
    unsigned int maxCycles;
};

} // namespace NVM::Simulation