#pragma once

#include "MemoryStack.h"

#include <memory>
#include <vector>

namespace NVM {

class RTSimStack : public MemoryStack {
    public:
    RTSimStack() = default;
    ~RTSimStack() = default;
    RTSimStack(RTSimStack&&) = default;
    RTSimStack(const RTSimStack&) = delete;
    RTSimStack& operator=(RTSimStack) = delete;

    void addLayer(std::unique_ptr<MemoryLayer> layer) final;
    MemoryLayer* getTopLayer() const final;

    private:
    std::vector<std::unique_ptr<MemoryLayer>> layers;
};

} // namespace NVM