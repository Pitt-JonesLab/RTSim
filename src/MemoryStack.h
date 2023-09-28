#pragma once

#include "src/MemoryLayer.h"

namespace NVM {

/*
 * The MemoryStack holds all of the actors in the RTSim simulation
 *
 * The actors are organized into layers that correspond to physical memory
 * layout (rank, bank, subarray)
 *
 * Each actor can send messages either upwards or
 * downwards
 */
class MemoryStack {
    public:
    MemoryStack();

    /**
     * Adds a MemoryLayer to the bottom of the stack
     */
    virtual void addLayer(const MemoryLayer& layer) = 0;

    /*
     *   Processes events from the top layer and their responses
     */
    virtual void processEvents() = 0;
};

} // namespace NVM