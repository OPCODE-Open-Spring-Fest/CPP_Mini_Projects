#pragma once
#include <cstddef>

struct IRandom {
    virtual ~IRandom() = default;
    virtual std::size_t nextIndex(std::size_t upperExclusive) = 0; // [0, upperExclusive)
};
