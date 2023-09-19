#pragma once

#include "src/MemoryLayer.h"

#include <memory>
#include <vector>

namespace NVM {

class RTSimLayer : public MemoryLayer {
    public:
    void addComponent(std::unique_ptr<MemoryComponent> component) final;
    MemoryComponent* getComponent() const final;

    private:
    std::vector<std::unique_ptr<MemoryComponent>> components;
};

} // namespace NVM