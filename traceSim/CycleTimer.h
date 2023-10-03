#pragma once

namespace NVM {

class CycleTimer {
    public:
    CycleTimer(ncycle_t max);

    bool cycle(ncycle_t cycles);
};

} // namespace NVM