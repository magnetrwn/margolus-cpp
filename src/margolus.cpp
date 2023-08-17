#include "margolus.hpp"


Margolus::Margolus(const size_t width, const size_t height, const std::vector<std::string>& transforms)
    : width_(width),
      height_(height),
      mt(static_cast<ulong>(std::time(nullptr))) {

    for (std::vector<std::string>::const_iterator it = transforms.begin(); it != transforms.end(); it++)
        blockTransforms.push_back(parseTransform(*it));

    grid.resize(height_);
    for (size_t i = 0; i < height_; i++)
        grid[i].resize(width_);

    offset = 0;

    fillRect(0, 0, width_ - 1, height_ - 1, DOWN);
}

Margolus::transform Margolus::parseTransform(const std::string& string) {
    if (string == "INVERT")
        return INVERT;
    else if (string == "ROTATE_90_LEFT")
        return ROTATE_90_LEFT;
    else if (string == "ROTATE_90_RIGHT")
        return ROTATE_90_RIGHT;
    else if (string == "ROTATE_180")
        return ROTATE_180;
    else
        throw std::runtime_error("Failed to parse transform from string.");
}

const std::deque<std::deque<bool>> Margolus::getGrid() const {
    return grid;
}

const std::pair<size_t, size_t> Margolus::getSize() const {
    return std::pair<size_t, size_t>(width_, height_);
}

bool Margolus::getOffset() const {
    return (bool) offset % 2;
}

void Margolus::fillRect(size_t x1, size_t y1, size_t x2, size_t y2, const fillState state, const double noise) {
    if (x1 > x2)
        std::swap(x1, x2);
    if (y1 > y2)
        std::swap(y1, y2);

    x2 += 1;
    y2 += 1;

    if (x2 >= width_ or y2 >= height_) {
        x2 = width_ - 1;
        y2 = height_ - 1;
    }

    for (size_t i = y1; i < y2; i++)
        for (size_t j = x1; j < x2; j++)
            switch (state) {
                case UP:
                    grid[i][j] = true;
                    break;
                case DOWN:
                    grid[i][j] = false;
                    break;
                case TOGGLE:
                    grid[i][j] = !grid[i][j];
                    break;
                case NOISE:
                    noisefn = std::bernoulli_distribution(noise);
                    grid[i][j] = noisefn(mt);
                    break;
                case NOISE_TOGGLE:
                    noisefn = std::bernoulli_distribution(noise);
                    if (noisefn(mt))
                        grid[i][j] = !grid[i][j];
                    break;
            }
}

void Margolus::fillPoint(size_t x1, size_t y1, const fillState state, const double noise) {
    fillRect(x1, y1, x1, y1, state, noise);
}

void Margolus::applyTransforms(bool block[4], bool invert) {
    for (std::vector<transform>::iterator it = blockTransforms.begin(); it != blockTransforms.end(); it++) {
        switch (*it) {
            case INVERT:
                invertBlock(block);
                break;
            case ROTATE_90_LEFT:
                if (invert)
                    rotate90RightBlock(block);
                else
                    rotate90LeftBlock(block);
                break;
            case ROTATE_90_RIGHT:
                if (invert)
                    rotate90LeftBlock(block);
                else
                    rotate90RightBlock(block);
                break;
            case ROTATE_180:
                rotate180Block(block);
                break;
        }
    }
}

void Margolus::step(stepDirection move) {
    bool active[4];
    size_t activeSum;

    if (move == BACKWARD)
        offset = 1 - offset;

    for (size_t i = 0; i < height_; i += 2) {
        for (size_t j = 0; j < width_; j += 2) {
            active[0] = (size_t) grid[(offset + i) % height_][(offset + j) % width_];
            active[1] = (size_t) grid[(offset + i) % height_][(offset + j + 1) % width_];
            active[2] = (size_t) grid[(offset + i + 1) % height_][(offset + j) % width_];
            active[3] = (size_t) grid[(offset + i + 1) % height_][(offset + j + 1) % width_];

            activeSum = (size_t) active[0] + active[1] + active[2] + active[3];
            bool changed = false;

            if (activeSum == 3 and move == FORWARD) {
                //rotate180Block(active);
                //invertBlock(active);
                applyTransforms(active);
                changed = true;

            } else if (activeSum == 1 and move == BACKWARD) {
                //rotate180Block(active);
                //invertBlock(active);
                applyTransforms(active, true);
                changed = true;

            } else if (activeSum != 2) {
                invertBlock(active);
                changed = true;
            }

            if (changed) {
                grid[(offset + i) % height_][(offset + j) % width_] = active[0];
                grid[(offset + i) % height_][(offset + j + 1) % width_] = active[1];
                grid[(offset + i + 1) % height_][(offset + j) % width_] = active[2];
                grid[(offset + i + 1) % height_][(offset + j + 1) % width_] = active[3];
            }
        }
    }

    if (move == FORWARD)
        offset = 1 - offset;
}

void Margolus::invertBlock(bool block[4]) {
    for (size_t i = 0; i < 4; i++)
        block[i] = !block[i];
}

void Margolus::rotate90LeftBlock(bool block[4]) {
    std::swap(block[2], block[0]);
    std::swap(block[0], block[1]);
    std::swap(block[1], block[3]);
}

void Margolus::rotate90RightBlock(bool block[4]) {
    std::swap(block[1], block[3]);
    std::swap(block[0], block[1]);
    std::swap(block[2], block[0]);
}

void Margolus::rotate180Block(bool block[4]) {
    std::swap(block[0], block[3]);
    std::swap(block[1], block[2]);
}
