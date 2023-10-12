#pragma once

#include <string>
#include <vector>

namespace NVM::Memory {

class StatValue {};

class StatBlock {
    public:

    private:
    std::string tag;
    std::vector<StatValue> values;
};

} // namespace NVM::Memory