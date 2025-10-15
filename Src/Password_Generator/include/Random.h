#pragma once
#include "IRandom.h"
#include <random>

class MTRandom final : public IRandom {
public:
    explicit MTRandom(unsigned int seed = std::random_device{}());
    std::size_t nextIndex(std::size_t upperExclusive) override;

private:
    std::mt19937 engine_;
};
