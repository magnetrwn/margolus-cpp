#include <iostream>
#include <array>
#include <stdexcept>
#include <vector>
#include <string>
#include <unistd.h>

#include "margolus.hpp"
#include "file.hpp"
#include "render.hpp"
#include "ruletool.hpp"
#include "util.hpp"

#include "popl.hpp"


int main(int argc, char **argv) {

    try {

        std::string load_file, save_file, renderer = "ANSI";
        bool show_help = false,
             run_animated = false,
             create_new = false,
             input_to_stdin = false,
             output_to_stdout = false,
             odd_flips_color = false;
        size_t new_width = DEFAULT_NEW_WIDTH, new_height = DEFAULT_NEW_HEIGHT;
        long iter = 0;

        popl::OptionParser options("Available command line options");

        options.add<popl::Switch>("h", "help", "Display help message.", &show_help);

        options.add<popl::Switch>("n", "new", "Create an empty grid.", &create_new);
        options.add<popl::Value<size_t>>("W", "width", "horizontal size of the new grid", DEFAULT_NEW_WIDTH, &new_width);
        options.add<popl::Value<size_t>>("H", "height", "vertical size of the new grid", DEFAULT_NEW_HEIGHT, &new_height);

        options.add<popl::Value<std::string>>("l", "load", "Path to the file to load.", load_file, &load_file);
        options.add<popl::Switch>("L", "load-stdin", "Load input file from STDIN.", &input_to_stdin);
        options.add<popl::Value<std::string>>("s", "save", "Path to the file to save, once finished.", save_file, &save_file);
        options.add<popl::Switch>("S", "save-stdout", "Show output file on STDOUT.", &output_to_stdout);

        options.add<popl::Value<long>>("i", "iters", "Iterations to run, positive forwards, negative backwards.", iter, &iter);
        options.add<popl::Value<std::string>>("r", "render", "Engine to use to render grid, options: q, ANSI, GIF, SDL2.", renderer, &renderer);
        options.add<popl::Switch>("o", "odd-flip", "Flip render colors on odd iterations.", &odd_flips_color);
        options.add<popl::Switch>("a", "animated", "Run with a 75ms step animation.", &run_animated);

        // Arguments: new grid creation

        // Arguments: new ruleset creation

        options.parse(argc, argv);

        if (show_help) {
            std::cout << options << std::endl;
            return 0;
        }

        if (load_file.empty() and !input_to_stdin)
            throw std::runtime_error("No input filename provided, use -L for STDIN.");

        if (output_to_stdout and run_animated)
            throw std::runtime_error("Not running animated with output to STDOUT.");

        if (renderer != "q" and renderer != "ANSI" and renderer != "GIF" and renderer != "SDL2")
            throw std::runtime_error("No such known renderer, options: q, ANSI, GIF, SDL2.");


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

        MargolusFile::writeMargolus(marg, save_file);


    } catch (const std::runtime_error &re) {
        std::cerr << "\x1B[93;01mError\x1B[0m: " << re.what() << std::endl;
        return 2;

    } catch (const std::exception &ex) {
        std::cerr << "\x1B[91;01mException\x1B[0m: " << ex.what() << std::endl;
        return 1;
    }

    return 0;
}
