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

    std::string loadFile, saveFile, renderer = "ANSI";
    bool showHelp = false,
            runAnimated = false,
            createNew = false,
            inputToStdin = false,
            outputToStdout = false,
            oddFlipsColor = false;
    size_t newWidth = DEFAULT_NEW_WIDTH, newHeight = DEFAULT_NEW_HEIGHT;
    long iter = 0;

    popl::OptionParser options("Available command line options");

    options.add<popl::Switch>("h", "help", "Display help message.", &showHelp);

    options.add<popl::Switch>("n", "new", "Create an empty grid.", &createNew);
    options.add<popl::Value<size_t>>("W", "width", "horizontal size of the new grid", newWidth, &newWidth);
    options.add<popl::Value<size_t>>("H", "height", "vertical size of the new grid", newHeight, &newHeight);

    options.add<popl::Value<std::string>>("l", "load", "Path to the file to load.", loadFile, &loadFile);
    options.add<popl::Switch>("L", "load-stdin", "Load input file from STDIN.", &inputToStdin);
    options.add<popl::Value<std::string>>("s", "save", "Path to the file to save, once finished.", saveFile, &saveFile);
    options.add<popl::Switch>("S", "save-stdout", "Show output file on STDOUT.", &outputToStdout);

    options.add<popl::Value<long>>("i", "iters", "Iterations to run, positive forwards, negative backwards.", iter, &iter);
    options.add<popl::Value<std::string>>("r", "render", "Engine to use to render grid, options: q, ANSI, GIF, SDL2.", renderer, &renderer);
    options.add<popl::Switch>("o", "odd-flip", "Flip render colors on odd iterations.", &oddFlipsColor);
    options.add<popl::Switch>("a", "animated", "Run with a 75ms step animation.", &runAnimated);

    // Arguments: new grid creation

    // Arguments: new ruleset creation

    try {
        options.parse(argc, argv);

        if (showHelp) {
            std::cout << options << std::endl;
            return 0;
        }

        if (loadFile.empty() and !inputToStdin)
            throw std::invalid_argument("No input filename provided, use -L for STDIN.");

        if (outputToStdout and runAnimated)
            throw std::invalid_argument("Not running animated with output to STDOUT.");

        if (renderer != "q" and renderer != "ANSI" and renderer != "GIF" and renderer != "SDL2")
            throw std::invalid_argument("No such known renderer, options: q, ANSI, GIF, SDL2.");


        std::unique_ptr<Margolus> loadedMargPtr = MargolusFile::readMargolus(loadFile);
        Margolus& marg = *loadedMargPtr;
        Margolus::stepDirection direction = Margolus::FORWARD;

        if (iter < 0) {
            iter = -iter;
            direction = Margolus::BACKWARD;
        }

        for (long i = 0; i < iter; i++) {
            if (runAnimated and renderer == "ANSI") {
                MargolusRender::basicANSI(
                    marg.getGrid(),
                    oddFlipsColor and marg.getOffset(),
                    inputToStdin ? "(STDIN)" : loadFile
                );
                usleep(1000 * 75);
            }
            marg.step(direction);
        }

        if (renderer == "ANSI" and !outputToStdout)
            MargolusRender::basicANSI(
                marg.getGrid(),
                oddFlipsColor and marg.getOffset(),
                inputToStdin ? "(STDIN)" : loadFile
            );

        if (!saveFile.empty() or outputToStdout)
            MargolusFile::writeMargolus(marg, saveFile);


    } catch (const popl::invalid_option &ex) {
        std::cerr << "\x1B[93;01mArgument error\x1B[0m: " << ex.what() << std::endl;
        return 1;

    } catch (const std::invalid_argument &ex) {
        std::cerr << "\x1B[93;01mArgument error\x1B[0m: " << ex.what() << std::endl;
        return 1;

    } catch (const std::runtime_error &ex) {
        std::cerr << "\x1B[93;01mRuntime error\x1B[0m: " << ex.what() << std::endl;
        return 2;

    } catch (const std::exception &ex) {
        std::cerr << "\x1B[91;01mCaught exception\x1B[0m: " << ex.what() << std::endl;
        return 3;
    }

    return 0;
}
