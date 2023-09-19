#pragma once

#include "src/EventQueue.h"
#include "src/MemoryComponent.h"
#include "src/Request.h"

#include <memory>

namespace NVM {

/**
 * MemoryLayers hold simulation actors of the same physical layer (rank, bank,
 * subarray)
 *
 * The layer can send messages to actors, and send responses to higher or lower
 * layers
 */
class MemoryLayer {
    public:
    MemoryLayer() = default;
    virtual ~MemoryLayer() = default;

    /**
     * Adds a component to the layer
     *
     * @param component Component to be added
     */
    virtual void addComponent(std::unique_ptr<MemoryComponent> component) = 0;

    /**
     * TODO this should take an address as well (should it???)
     */
    virtual MemoryComponent* getComponent() const = 0;
};

template<typename T>
T handleRequest(const MemoryLayer& layer, const Request<T>& request) {
    auto component = layer.getComponent();
    return request(*component);
}

} // namespace NVM