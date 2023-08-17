#include <iostream>
#include <vector>
#include <string>
#include <unistd.h>

#include "margolus.hpp"
#include "render.hpp"


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

    const std::vector<std::string> transforms = TXTUtil::readTXT(SETTINGS_FILE_PATH);
    Margolus crt(width, height, transforms);

    // Pulsing cross
    crt.fillPoint(5, 5, Margolus::UP);
    crt.fillPoint(6, 6, Margolus::UP);
    crt.fillPoint(7, 7, Margolus::UP);
    crt.fillPoint(8, 8, Margolus::UP);
    crt.fillPoint(8, 5, Margolus::UP);
    crt.fillPoint(7, 6, Margolus::UP);
    crt.fillPoint(6, 7, Margolus::UP);
    crt.fillPoint(5, 8, Margolus::UP);

    // Rising spaceship
    crt.fillPoint(15, 8, Margolus::UP);
    crt.fillPoint(16, 7, Margolus::UP);
    crt.fillPoint(17, 7, Margolus::UP);
    crt.fillPoint(18, 8, Margolus::UP);

    // Bouncing spaceship
    crt.fillPoint(23, 5, Margolus::UP);
    crt.fillPoint(24, 6, Margolus::UP);
    crt.fillPoint(25, 6, Margolus::UP);
    crt.fillPoint(26, 5, Margolus::UP);

    // Bounce point
    crt.fillPoint(28, 12, Margolus::UP);
    crt.fillPoint(29, 13, Margolus::UP);
    crt.fillPoint(30, 14, Margolus::UP);

    while (true) {
        std::cout << "\033[2J\033[H";
        crt.step(Margolus::FORWARD);
        RenderGrid::basicANSI(crt.getGrid(), crt.getOffset());
        usleep(1000 * 75);
    }
    /*
    crt.fillRect(width/2 - 3, height/2 - 2, width/2 + 3, height/2 + 2, Margolus::NOISE, 0.5);

    while (true) {
        std::cout << "\033[2J\033[H";
        RenderGrid::basicANSI(crt.getGrid());
        usleep(1000 * 650);
        for (size_t q = 0; q < 50; q++) {
            std::cout << "\033[2J\033[H";
            crt.step(Margolus::FORWARD);
            RenderGrid::basicANSI(crt.getGrid());
            usleep(1000 * 75);
        }
        crt.fillRect(width/2 - 3, height/2 - 2, width/2 + 3, height/2 + 2, Margolus::NOISE_TOGGLE, 0.875);
    }
    */
    return 0;
}
