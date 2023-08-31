#ifndef MARGOLUS_HEADER_
#define MARGOLUS_HEADER_

#include <cstddef>
#include <string>
#include <vector>
#include <deque>
#include <array>
#include <random>
#include <ctime>
#include <algorithm>
#include <stdexcept>
#include <memory>

#include "rbitset.hpp"

#include "ruletool.hpp"
#include "util.hpp"


class Margolus {
protected:
    const size_t width_, height_;
    size_t offset_;
    const std::array<std::array<bool, 4>, 16> transforms_;

    std::unique_ptr<RB::RuntimeBitset<>[]> grid;

    std::mt19937 mt;
    std::bernoulli_distribution noisefn;

    void applyTransforms(std::array<bool, 4>& block, const bool invert = false);
    static bool blockCompare(const std::array<bool, 4>& a, const std::array<bool, 4>& b);

public:
    Margolus(const size_t width, const size_t height, const std::array<std::array<bool, 4>, 16>& transforms, const size_t offset = 0);

    RB::RuntimeBitset<>& operator[](const size_t y);
    const RB::RuntimeBitset<>& operator[](const size_t y) const;

    const std::pair<size_t, size_t> getSize() const;
    const std::array<std::array<bool, 4>, 16>& getTransforms() const;
    bool getOffset() const;

    enum stepDirection { FORWARD, BACKWARD };
    void step(stepDirection move = FORWARD);

    enum fillState { UP, DOWN, TOGGLE, NOISE, NOISE_TOGGLE };
    void fillPoint(size_t x, size_t y, const fillState state, const double noise = 0.5);
    void fillRect(size_t x1, size_t y1, size_t x2, size_t y2, const fillState state, const double noise = 0.5);
};

#endif