#include "render.hpp"

void RenderGrid::basicANSI(const std::deque<std::deque<bool>>& grid, bool invert) {
    size_t up = 0, down = 0, width = grid[0].size(), height = grid.size();
    for (size_t i = 0; i < height; i++) {
        for (size_t j = 0; j < width; j++) {
            if ((invert and !grid[i][j]) or (!invert and grid[i][j])) {
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