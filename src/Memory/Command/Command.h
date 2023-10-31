#pragma once

namespace NVM::Memory {

class Command {
    public:
    virtual void setParent(Command* parent) = 0;

    virtual void notify() = 0;

    virtual bool isDone() const = 0;

    virtual void cycle(unsigned int) = 0;
};

} // namespace NVM::Memory