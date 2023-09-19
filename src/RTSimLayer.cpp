#include "src/RTSimLayer.h"

using namespace NVM;

void RTSimLayer::addComponent(std::unique_ptr<MemoryComponent> component) {
    components.push_back(std::move(component));
}

MemoryComponent* RTSimLayer::getComponent() const {
    return (components.size()) ? components[0].get() : nullptr;
}