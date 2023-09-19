#include "src/RTSimStack.h"

using namespace NVM;

void RTSimStack::addLayer(std::unique_ptr<MemoryLayer> layer) {
    layers.push_back(std::move(layer));
}

MemoryLayer* RTSimStack::getTopLayer() const {
    return (layers.size()) ? layers[0].get() : nullptr;
}