#include <deque>
#include <random>
#include <algorithm>
#include <stdexcept>

#include <iostream>


class Margolus {
private:
    const size_t width_, height_;

    std::deque<std::deque<bool>> grid;
    size_t offset;

    std::mt19937 mt;
    std::bernoulli_distribution noisefn;

    //enum actions { INVERT, ROTATE_90_LEFT, ROTATE_90_RIGHT, ROTATE_180 };
    //std::deque<actions> onMatchingBlock;

public:
    Margolus(size_t width, size_t height);

    enum stepDirection { FORWARD, BACKWARD };
    void step(stepDirection move = FORWARD);

    enum fillState { UP, DOWN, TOGGLE, NOISE, NOISE_TOGGLE };
    void fillRect(size_t x1, size_t y1, size_t x2, size_t y2, fillState state, double noise = 0.5);

    void render() const;
};
