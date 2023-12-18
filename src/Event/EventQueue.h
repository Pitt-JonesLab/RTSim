#pragma once

#include <memory>

namespace NVM::Event {

class EventQueue {
    public:
    
    virtual bool issue() = 0;
};

}