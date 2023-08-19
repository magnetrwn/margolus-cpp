#ifndef MARGOLUS_HEADER_
#define MARGOLUS_HEADER_

#include <string>
#include <vector>
#include <deque>
#include <random>
#include <ctime>
#include <algorithm>
#include <stdexcept>

#include "util.hpp"


class Margolus {
private:
    const size_t width_, height_;

    std::deque<std::deque<bool>> grid;
    size_t offset;

    enum transform { INVERT, ROTATE_90_LEFT, ROTATE_90_RIGHT, ROTATE_180 };
    std::vector<transform> blockTransforms;

    std::mt19937 mt;
    std::bernoulli_distribution noisefn;

    void applyTransforms(bool block[4], bool invert = false);

public:
    Margolus(const size_t width, const size_t height, const std::vector<std::string>& transforms);
    const std::deque<std::deque<bool>> getGrid() const;
    const std::pair<size_t, size_t> getSize() const;
    bool getOffset() const;

    enum stepDirection { FORWARD, BACKWARD };
    void step(stepDirection move = FORWARD);

    enum fillState { UP, DOWN, TOGGLE, NOISE, NOISE_TOGGLE };
    void fillPoint(size_t x1, size_t y1, const fillState state, const double noise = 0.5);
    void fillRect(size_t x1, size_t y1, size_t x2, size_t y2, const fillState state, const double noise = 0.5);

    static transform parseTransform(const std::string& string);

    static void invertBlock(bool block[4]);
    static void rotate90LeftBlock(bool block[4]);
    static void rotate90RightBlock(bool block[4]);
    static void rotate180Block(bool block[4]);
};

#endif