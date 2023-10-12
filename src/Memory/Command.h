#pragma once

namespace NVM::Memory {

class Command {
    public:
    virtual void setParent(Command* parent) = 0;

    virtual void notify() = 0;
};

} // namespace NVM::Memory