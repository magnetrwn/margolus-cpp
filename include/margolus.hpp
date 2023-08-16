#ifndef MARGOLUS_HEADER_
#define MARGOLUS_HEADER_

#include <deque>
#include <random>
#include <ctime>
#include <algorithm>
#include <stdexcept>


class Margolus {
private:
    const size_t width_, height_;

    std::deque<std::deque<bool>> grid;
    size_t offset;

    std::mt19937 mt;
    std::bernoulli_distribution noisefn;

    enum actions { INVERT, ROTATE_90_LEFT, ROTATE_90_RIGHT, ROTATE_180 };
    std::deque<actions> onMatchingBlock;

public:
    Margolus(size_t width, size_t height);
    const std::deque<std::deque<bool>> getGrid() const;

    enum stepDirection { FORWARD, BACKWARD };
    void step(stepDirection move = FORWARD);

    enum fillState { UP, DOWN, TOGGLE, NOISE, NOISE_TOGGLE };
    void fillRect(size_t x1, size_t y1, size_t x2, size_t y2, fillState state, double noise = 0.5);
};

#endif