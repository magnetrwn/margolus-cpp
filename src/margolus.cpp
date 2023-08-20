#include "margolus.hpp"
#include "marg_ruletool.hpp"


Margolus::Margolus(const size_t width, const size_t height, const std::array<std::array<bool, 4>, 16>& transforms, const size_t offset)
    : width_(width),
      height_(height),
      offset_(offset),
      transforms_(transforms),
      mt(static_cast<ulong>(std::time(nullptr))) {

    if (width_ % 2 != 0 or height_ % 2 != 0)
        throw std::runtime_error("Width and/or height not even.");

    if (width_ < 2 or height_ < 2)
        throw std::runtime_error("Width and/or height too small.");

    if (width_ > MAX_SCREEN_ANY or height_ > MAX_SCREEN_ANY)
        throw std::runtime_error("Width and/or height too big.");

    grid.resize(height_);
    for (size_t i = 0; i < height_; i++)
        grid[i].resize(width_);

    fillRect(0, 0, width_ - 1, height_ - 1, DOWN);
}

const std::deque<std::deque<bool>>& Margolus::getGrid() const {
    return grid;
}

const std::pair<size_t, size_t> Margolus::getSize() const {
    return std::pair<size_t, size_t>(width_, height_);
}

const std::array<std::array<bool, 4>, 16>& Margolus::getTransforms() const {
    return transforms_;
}

bool Margolus::getOffset() const {
    return (bool) offset_ % 2;
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

void Margolus::applyTransforms(std::array<bool, 4>& block, const bool invert) {
    static const std::array<std::array<bool, 4>, 16> binary_up = MargolusRuletool::generateTransforms(MargolusRuletool::_BINARY);

    const std::array<std::array<bool, 4>, 16>& from = (invert ? transforms_ : binary_up);
    const std::array<std::array<bool, 4>, 16>& to = (invert ? binary_up : transforms_);

    for (size_t i = 0; i < 16; i++)
        if (blockCompare(from[i], block)) {
            std::copy(to[i].begin(), to[i].end(), block.begin());
            return;
        }
}

bool Margolus::blockCompare(const std::array<bool, 4>& a, const std::array<bool, 4>& b) {
    for (size_t i = 0; i < 4; i++)
        if (a[i] != b[i])
            return false;
    return true;
}

void Margolus::step(stepDirection move) {
    if (move == BACKWARD)
        offset_ = 1 - offset_;

    for (size_t i = 0; i < height_; i += 2) {
        for (size_t j = 0; j < width_; j += 2) {
            std::array<bool, 4> active = {{
                grid[(offset_ + i) % height_][(offset_ + j) % width_],
                grid[(offset_ + i) % height_][(offset_ + j + 1) % width_],
                grid[(offset_ + i + 1) % height_][(offset_ + j) % width_],
                grid[(offset_ + i + 1) % height_][(offset_ + j + 1) % width_]
            }};

            //MargolusRender::basicANSI(grid);
            //std::cout << "before: " << active[0] << ", " << active[1] << ", " << active[2] << ", " << active[3] << std::endl;

            if (move == FORWARD)
                applyTransforms(active);
            else
                applyTransforms(active, true);

            //std::cout << "after: " << active[0] << ", " << active[1] << ", " << active[2] << ", " << active[3] << std::endl;
            //usleep(1000*500);

            grid[(offset_ + i) % height_][(offset_ + j) % width_] = active[0];
            grid[(offset_ + i) % height_][(offset_ + j + 1) % width_] = active[1];
            grid[(offset_ + i + 1) % height_][(offset_ + j) % width_] = active[2];
            grid[(offset_ + i + 1) % height_][(offset_ + j + 1) % width_] = active[3];

        }
    }

    if (move == FORWARD)
        offset_ = 1 - offset_;
}
