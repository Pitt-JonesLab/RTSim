#pragma once

namespace NVM {

/**
 * MemoryComponents represent physical components in an RTSim memory system
 *
 * They can handle requests sent between MemoryLayers, either creating responses
 * or propagating the request
 */
class MemoryComponent {
    public:
    MemoryComponent() = default;
    virtual ~MemoryComponent() = default;

    virtual void cycle(unsigned int cycles) = 0;
};

} // namespace NVM