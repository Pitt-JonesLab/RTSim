#pragma once

#include "src/MemoryLayer.h"

#include <memory>

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
    MemoryStack() = default;
    virtual ~MemoryStack() = default;

    /**
     * Adds a MemoryLayer to the bottom of the stack
     *
     * @param layer Layer to be added
     */
    virtual void addLayer(std::unique_ptr<MemoryLayer> layer) = 0;

    /**
     * Returns the top layer of the stack
     *
     * @return Top layer of the stack
     */
    virtual MemoryLayer* getTopLayer() const = 0;
};

template<typename T>
T handleRequest(const MemoryStack& stack, Request<T> request) {
    return handleRequest(*stack.getTopLayer(), request);
}

} // namespace NVM