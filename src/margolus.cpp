#include "margolus.hpp"


Margolus::Margolus(size_t width, size_t height)
    : width_(width), height_(height), mt(static_cast<ulong>(std::time(nullptr))) {

    grid.resize(height_);
    for (size_t i = 0; i < height_; i++)
        grid[i].resize(width_);

    offset = 0;

    fillRect(0, 0, width_ - 1, height_ - 1, DOWN);
}

const std::deque<std::deque<bool>> Margolus::getGrid() const {
    return grid;
}

void Margolus::fillRect(size_t x1, size_t y1, size_t x2, size_t y2, fillState state, double noise) {
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

void Margolus::step(stepDirection move) {
    for (size_t s = 0; s < 2; s++) {
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

                if (activeSum == 3 and move == FORWARD) {
                    // Turn clockwise
                    std::swap(active[1], active[3]);
                    std::swap(active[0], active[1]);
                    std::swap(active[2], active[0]);
                }

                if (activeSum == 1 and move == BACKWARD) {
                    // Turn counter-clockwise
                    std::swap(active[2], active[0]);
                    std::swap(active[0], active[1]);
                    std::swap(active[1], active[3]);
                }

                if (activeSum != 2) {
                    // Invert values
                    grid[(offset + i) % height_][(offset + j) % width_] = !active[0];
                    grid[(offset + i) % height_][(offset + j + 1) % width_] = !active[1];
                    grid[(offset + i + 1) % height_][(offset + j) % width_] = !active[2];
                    grid[(offset + i + 1) % height_][(offset + j + 1) % width_] = !active[3];
                }
            }
        }

        if (move == FORWARD)
            offset = 1 - offset;
    }
}
