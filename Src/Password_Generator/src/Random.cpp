#include "Random.h"
#include <stdexcept>

MTRandom::MTRandom(unsigned int seed) : engine_{seed} {}

std::size_t MTRandom::nextIndex(std::size_t upperExclusive) {
    if (upperExclusive == 0) throw std::invalid_argument("upperExclusive must be > 0");
    std::uniform_int_distribution<std::size_t> dist(0, upperExclusive - 1);
    return dist(engine_);
}
