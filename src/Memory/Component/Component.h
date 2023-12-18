#pragma once

namespace NVM::Memory {

class Component {
    public:

    virtual void cycle() = 0;

    virtual unsigned int getCurrentCycle() const = 0;
};

}