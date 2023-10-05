#pragma once

namespace NVM::Simulation {

class MemorySystem;

class TraceCommand {
    public:
    /**
     * Issues this command to the MemorySystem
     *
     * @param memory MemorySystem to receive this command
     *
     * @return True if successful, false otherwise
     */
    virtual bool issue(MemorySystem* memory) = 0;
};

} // namespace NVM::Simulation