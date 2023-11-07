#pragma once

#include "Memory/Component.h"

namespace NVM::Memory {

class LowLevelComponent : public Component {
    virtual Command* activate(uint64_t address) = 0;
    virtual Command* precharge() = 0;
    virtual Command* refresh() = 0;
};

}