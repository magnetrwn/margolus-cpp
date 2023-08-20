#include <iostream>
#include <array>
#include <vector>
#include <string>
#include <unistd.h>

#include "margolus.hpp"
#include "marg_render.hpp"
#include "util.hpp"

#include "popl.hpp"


void demo1(Margolus crt) {
    size_t width = crt.getSize().first;
    size_t height = crt.getSize().second;

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

    for (size_t q = 0; q < 150; q++) {
        crt.step(Margolus::FORWARD);
        MargolusRender::basicANSI(crt.getGrid(), crt.getOffset());
        usleep(1000 * 75);
    }

    crt.fillRect(0, 0, width - 1, height - 1, Margolus::DOWN);
}

void demo2(Margolus crt) {
    size_t width = crt.getSize().first;
    size_t height = crt.getSize().second;

    crt.fillRect(width/2 - 3, height/2 - 2, width/2 + 3, height/2 + 2, Margolus::NOISE, 0.5);

    for (size_t p = 0; p < 4; p++) {
        MargolusRender::basicANSI(crt.getGrid());
        usleep(1000 * 650);
        for (size_t q = 0; q < 50; q++) {
            crt.step(Margolus::FORWARD);
            MargolusRender::basicANSI(crt.getGrid(), crt.getOffset());
            usleep(1000 * 75);
        }
        crt.fillRect(width/2 - 3, height/2 - 2, width/2 + 3, height/2 + 2, Margolus::NOISE_TOGGLE, 0.875);
    }

    crt.fillRect(0, 0, width - 1, height - 1, Margolus::DOWN);
}

int main(int argc, char **argv) {
    bool show_help = false, run_demo = false, run_animated = false;
    size_t width = DEFAULT_WIDTH, height = DEFAULT_HEIGHT;
    long iter = 0;

    popl::OptionParser options("Available command line options");
    options.add<popl::Switch>("h", "help", "display help message", &show_help);
    options.add<popl::Value<size_t>>("W", "width", "horizontal size of the grid", width, &width);
    options.add<popl::Value<size_t>>("H", "height", "vertical size of the grid", height, &height);
    options.add<popl::Value<long>>("i", "iters", "iterations to run, positive forwards, negative backwards", iter, &iter);
    options.add<popl::Switch>("a", "animated", "run with a 75ms step animation", &run_animated);
    options.add<popl::Switch>("", "demo", "run the demo loop", &run_demo);

    options.parse(argc, argv);

    if (show_help) {
        std::cout << options << std::endl;
        return 0;
    }

    // Here are the conversions for Critters,
    // 1 2   Each block is separated in the following
    // 3 4   four bools, in this order.
    // Then the ruleset determines, in rising binary
    // order, how to convert from one state to the
    // next.
    // E.g.: {0, 0, 0, 0} -> ruleset[0]
    //       {0, 0, 0, 1} -> ruleset[1]
    //       {0, 0, 1, 0} -> ruleset[2]
    //       ...
    const std::array<std::array<bool, 4>, 16> critters_ruleset = {{
        {1, 1, 1, 1},
        {1, 1, 1, 0},
        {1, 1, 0, 1},
        {0, 0, 1, 1},

        {1, 0, 1, 1},
        {0, 1, 0, 1},
        {0, 1, 1, 0},
        {0, 0, 0, 1},

        {0, 1, 1, 1},
        {1, 0, 0, 1},
        {1, 0, 1, 0},
        {0, 0, 1, 0},

        {1, 1, 0, 0},
        {0, 1, 0, 0},
        {1, 0, 0, 0},
        {0, 0, 0, 0}
    }};

    const std::array<std::array<bool, 4>, 16> billiard_ball_ruleset = {{
        {0, 0, 0, 0},
        {1, 0, 0, 0},
        {0, 1, 0, 0},
        {0, 0, 1, 1},
        {0, 0, 1, 0},
        {0, 1, 0, 1},
        {1, 0, 0, 1},
        {0, 1, 1, 1},
        {0, 0, 0, 1},
        {0, 1, 1, 0},
        {1, 0, 1, 0},
        {1, 0, 1, 1},
        {1, 1, 0, 0},
        {1, 1, 0, 1},
        {1, 1, 1, 0},
        {1, 1, 1, 1}
    }};


    if (run_demo) {
        Margolus crt(DEFAULT_WIDTH, DEFAULT_HEIGHT, billiard_ball_ruleset);

        while (true) {
            demo1(crt);
            demo2(crt);
        }

    } else {
        Margolus crt(width, height, billiard_ball_ruleset);
        crt.fillPoint(6, 1, Margolus::UP);
        crt.fillPoint(5, 2, Margolus::UP);
        crt.fillPoint(5, 3, Margolus::UP);
        crt.fillPoint(6, 4, Margolus::UP);

        Margolus::stepDirection direction = Margolus::FORWARD;

        if (iter < 0) {
            iter = -iter;
            direction = Margolus::BACKWARD;
        }

        for (long i = 0; i < iter; i++) {
            if (run_animated) {
                MargolusRender::basicANSI(crt.getGrid());
                usleep(1000 * 75);
            }
            crt.step(direction);
        }
        MargolusRender::basicANSI(crt.getGrid());
    }

    return 0;
}
