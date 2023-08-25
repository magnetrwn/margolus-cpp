#include "render.hpp"

void MargolusRender::basicANSI(const std::deque<std::deque<bool>>& grid, const bool invert, const std::string& comment) {
    size_t up = 0, down = 0, width = grid[0].size(), height = grid.size();
    std::string upStr, downStr, commentPadding;

    std::cout << "\033[2J\033[H";
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
    upStr = std::to_string(up);
    downStr = std::to_string(down);
    commentPadding = std::string(
        std::max(1, static_cast<int>(width - upStr.size() - 1 - downStr.size() - 1 - comment.size())), ' ');

    std::cout << "\x1B[97;01m"
              << upStr
              << " \x1B[0m\x1B[90m"
              << downStr
              << " \x1B[0m"
              << commentPadding
              << "\x1B[97;01m"
              << comment
              << "\x1B[0m"
              << std::endl;
}
