#include <iostream>
#include <array>
#include <stdexcept>
#include <vector>
#include <string>
#include <unistd.h>

#include "margolus.hpp"
#include "marg_file.hpp"
#include "marg_render.hpp"
#include "marg_ruletool.hpp"
#include "util.hpp"

#include "popl.hpp"


int main(int argc, char **argv) {
    std::string load_file, save_file, renderer = "q";
    bool show_help = false, run_animated = false, output_to_stdout = false;
    bool odd_flips_color = false;
    long iter = 0;

    popl::OptionParser options("Available command line options");

    // Arguments: general
    options.add<popl::Switch>("h", "help", "Display help message.", &show_help);

    // Arguments: simulator commands
    options.add<popl::Value<std::string>>("l", "load", "Path to the file to load.", load_file, &load_file);
    options.add<popl::Value<std::string>>("s", "save", "Path to the file to save, once finished.", save_file, &save_file);
    options.add<popl::Switch>("S", "save-stdout", "Show output file on stdout instead of an actual file.", &output_to_stdout);
    options.add<popl::Value<long>>("i", "iters", "Iterations to run, positive forwards, negative backwards.", iter, &iter);
    options.add<popl::Value<std::string>>("r", "render", "Engine to use to render grid, options: q, ANSI, SDL2.", renderer, &renderer);
    options.add<popl::Switch>("o", "odd-flip", "Flip render colors on odd iterations.", &odd_flips_color);
    options.add<popl::Switch>("a", "animated", "Run with a 75ms step animation.", &run_animated);

    // Arguments: new grid creation

    // Arguments: new ruleset creation

    options.parse(argc, argv);

    if (show_help) {
        std::cout << options << std::endl;
        return 0;
    }

    if (load_file.empty())
        throw std::runtime_error("No filename provided.");

    if (renderer != "q" and renderer != "ANSI" and renderer != "SDL2")
        throw std::runtime_error("No such known renderer, options: q, ANSI, SDL2.");

    Margolus marg = MargolusFile::readMargolus(load_file);
    Margolus::stepDirection direction = Margolus::FORWARD;

    if (iter < 0) {
        iter = -iter;
        direction = Margolus::BACKWARD;
    }

    for (long i = 0; i < iter; i++) {
        if (run_animated and renderer == "ANSI") {
            MargolusRender::basicANSI(marg.getGrid(), odd_flips_color and marg.getOffset());
            usleep(1000 * 75);
        }
        marg.step(direction);
    }

    if (renderer == "ANSI" and !output_to_stdout)
        MargolusRender::basicANSI(marg.getGrid(), odd_flips_color and marg.getOffset());

    if (output_to_stdout) {
        std::vector<std::string> stdout_content = MargolusFile::fileStringsGenerator(marg);
        for (std::string& line : stdout_content)
            std::cout << line << std::endl;
    }

    if (!save_file.empty())
        MargolusFile::writeMargolus(save_file, marg);

    return 0;
}
