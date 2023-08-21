#include <iostream>
#include <array>
#include <vector>
#include <string>
#include <unistd.h>

#include "margolus.hpp"
#include "marg_file.hpp"
#include "marg_render.hpp"
#include "marg_ruletool.hpp"
#include "util.hpp"

#include "popl.hpp"


void demo_critters_periodic() {
    Margolus crt(DEFAULT_WIDTH, DEFAULT_HEIGHT, MargolusRuletool::generateTransforms(MargolusRuletool::CRITTERS));

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

    MargolusFile::writeMargolus("demo_critters_periodic.txt", crt);
    crt.fillRect(0, 0, width - 1, height - 1, Margolus::DOWN);
}

void demo_critters_spawns() {
    Margolus crt(DEFAULT_WIDTH, DEFAULT_HEIGHT, MargolusRuletool::generateTransforms(MargolusRuletool::CRITTERS));

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

    MargolusFile::writeMargolus("demo_critters_spawns.txt", crt);
    crt.fillRect(0, 0, width - 1, height - 1, Margolus::DOWN);
}

void demo_billiards_concave() {
    Margolus bb(DEFAULT_WIDTH, DEFAULT_HEIGHT, MargolusRuletool::generateTransforms(MargolusRuletool::BILLIARD_BALL));

    size_t width = bb.getSize().first;
    size_t height = bb.getSize().second;

    bb.fillRect(6, 4, 16, 5, Margolus::UP);
    bb.fillRect(6, 10, 16, 11, Margolus::UP);
    bb.fillRect(5, 4, 6, 11, Margolus::UP);

    bb.fillRect(8, 7, 16, 8, Margolus::NOISE, 0.3333);

    for (size_t q = 0; q < 220; q++) {
        bb.step(Margolus::FORWARD);
        MargolusRender::basicANSI(bb.getGrid());
        usleep(1000 * 75);
    }

    MargolusFile::writeMargolus("demo_billiards_concave.txt", bb);
    bb.fillRect(0, 0, width - 1, height - 1, Margolus::DOWN);
}

void demo_billiards_squiggle() {
    Margolus bb(DEFAULT_WIDTH, DEFAULT_HEIGHT, MargolusRuletool::generateTransforms(MargolusRuletool::BILLIARD_BALL));

    size_t width = bb.getSize().first;
    size_t height = bb.getSize().second;

    bb.fillRect(1, 0, 34, 1, Margolus::UP);
    bb.fillRect(1, 12, 34, 13, Margolus::UP);
    bb.fillRect(0, 1, 1, 12, Margolus::UP);
    bb.fillRect(34, 1, 35, 12, Margolus::UP);
    bb.fillRect(8, 2, 9, 6, Margolus::UP);
    bb.fillRect(26, 2, 27, 6, Margolus::UP);
    bb.fillRect(17, 6, 18, 11, Margolus::UP);

    bb.fillRect(3, 3, 6, 10, Margolus::NOISE);

    for (size_t q = 0; q < 150; q++) {
        bb.step(Margolus::FORWARD);
        MargolusRender::basicANSI(bb.getGrid());
        usleep(1000 * 75);
    }

    MargolusFile::writeMargolus("demo_billiards_squiggle.txt", bb);
    bb.fillRect(0, 0, width - 1, height - 1, Margolus::DOWN);
}

int main(int argc, char **argv) {
    bool show_help = false, run_demo = false, run_animated = false;
    bool odd_flips_color = false;
    size_t width = DEFAULT_WIDTH, height = DEFAULT_HEIGHT;
    long iter = 0;

    popl::OptionParser options("Available command line options");
    options.add<popl::Switch>("h", "help", "display help message", &show_help);
    options.add<popl::Value<size_t>>("W", "width", "horizontal size of the grid", width, &width);
    options.add<popl::Value<size_t>>("H", "height", "vertical size of the grid", height, &height);
    options.add<popl::Value<long>>("i", "iters", "iterations to run, positive forwards, negative backwards", iter, &iter);
    options.add<popl::Switch>("o", "odd-flip", "flip render colors on odd iterations", &odd_flips_color);
    options.add<popl::Switch>("a", "animated", "run with a 75ms step animation", &run_animated);
    options.add<popl::Switch>("", "demo", "run the demo loop", &run_demo);

    options.parse(argc, argv);

    if (show_help) {
        std::cout << options << std::endl;
        return 0;
    }

    if (run_demo) {
        while (true) {
            demo_billiards_squiggle();
            demo_critters_periodic();
            demo_billiards_concave();
            demo_critters_spawns();
        }
    } /*else {
        Margolus marg(width, height, billiard_ball_ruleset);
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
                MargolusRender::basicANSI(crt.getGrid(), odd_flips_color and crt.getOffset());
                usleep(1000 * 75);
            }
            crt.step(direction);
        }
        MargolusRender::basicANSI(crt.getGrid(), odd_flips_color and crt.getOffset());
    }*/

    /*Margolus bb(width, height, MargolusRuletool::generateTransforms(MargolusRuletool::BILLIARD_BALL));

    bb.fillRect(1, 0, 34, 1, Margolus::UP);
    bb.fillRect(1, 12, 34, 13, Margolus::UP);
    bb.fillRect(0, 1, 1, 12, Margolus::UP);
    bb.fillRect(34, 1, 35, 12, Margolus::UP);
    bb.fillRect(8, 2, 9, 6, Margolus::UP);
    bb.fillRect(26, 2, 27, 6, Margolus::UP);
    bb.fillRect(17, 6, 18, 11, Margolus::UP);

    bb.fillRect(3, 3, 6, 10, Margolus::NOISE);*/

    return 0;
}
