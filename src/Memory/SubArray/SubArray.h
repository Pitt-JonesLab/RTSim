#pragma once

#include "Memory/Component.h"

namespace NVM::Memory {

class SubArray : public Component {
    virtual Command* switchRow(unsigned int row) = 0;
};

} // namespace NVM::Memory