#pragma once

#include "MemoryComponent.h"

#include <functional>

namespace NVM {

template<typename T> using Request = std::function<T(MemoryComponent&)>;

} // namespace NVM