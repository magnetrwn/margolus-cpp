#include <iostream>
#include <deque>
#include <random>
#include <algorithm>
#include <stdexcept>

#include <unistd.h>

#define MAX_SCREEN_ANY 512
#define DEFAULT_WIDTH 36
#define DEFAULT_HEIGHT 14

class Critters {
private:
    const size_t width_, height_;

    std::deque<std::deque<bool>> grid;
    size_t offset;

    std::mt19937 mt;
    std::bernoulli_distribution noisefn;

public:
    Critters(size_t width, size_t height);

    enum stepDirection { FORWARD, BACKWARD };
    void step(stepDirection move = FORWARD);

    enum fillState { UP, DOWN, TOGGLE, NOISE, NOISE_TOGGLE };
    void fillRect(size_t x1, size_t y1, size_t x2, size_t y2, fillState state, double noise = 0.5);

    void render() const;
};

Critters::Critters(size_t width, size_t height)
    : width_(width), height_(height), mt(static_cast<ulong>(time(nullptr))) {

    grid.resize(height_);
    for (size_t i = 0; i < height_; i++)
        grid[i].resize(width_);

    offset = 0;

    fillRect(0, 0, width_ - 1, height_ - 1, DOWN);
}

void Critters::fillRect(size_t x1, size_t y1, size_t x2, size_t y2, fillState state, double noise) {
    if (x1 > x2)
        std::swap(x1, x2);
    if (y1 > y2)
        std::swap(y1, y2);
    if (x2 >= width_ or y2 >= height_)
        std::runtime_error("Fill coordinates out of bounds.");

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

void Critters::step(stepDirection move) {
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

void Critters::render() const {
    size_t up = 0, down = 0;
    for (size_t i = 0; i < height_; i++) {
        for (size_t j = 0; j < width_; j++) {
            if (grid[i][j]) {
                std::cout << "\x1B[97;01m$\x1B[0m";
                up++;
            } else {
                std::cout << "\x1B[90m$\x1B[0m";
                down++;
            }
        }
        std::cout << std::endl;
    }
    std::cout << "\x1B[97;01m" << up << " \x1B[0m\x1B[90m" << down << std::endl;
}

int main(int argc, char **argv) {
    size_t width, height;
    if (argc == 1) {
        width = DEFAULT_WIDTH;
        height = DEFAULT_HEIGHT;
    } else if (argc == 3) {
        width = std::stoul(std::string(argv[1]));
        height = std::stoul(std::string(argv[2]));
        if (width > MAX_SCREEN_ANY or height > MAX_SCREEN_ANY)
            throw std::runtime_error("Width and/or height too big.");
        if (width % 2 != 0 or height % 2 != 0 or width < 2 or height < 2)
            throw std::runtime_error("Width and/or height is small or not an even number.");
    } else
        throw std::runtime_error("Invalid width and/or height of the grid.");

    Critters crt(width, height);

    crt.fillRect(2, 2, width - 2, height - 2, Critters::UP);

    while (true) {
        std::cout << "\033[2J\033[H";
        crt.render();
        usleep(500000);
        for (size_t q = 0; q < 30; q++) {
            std::cout << "\033[2J\033[H";
            crt.step(Critters::FORWARD);
            crt.render();
            usleep(1000 * 75);
        }
        for (size_t q = 0; q < 30; q++) {
            std::cout << "\033[2J\033[H";
            crt.step(Critters::BACKWARD);
            crt.render();
            usleep(1000 * 75);
        }
    }
    return 0;
}