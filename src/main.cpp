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

    std::string loadFile;
    std::string saveFile;
    std::string renderer = DEFAULT_RENDERER;
    std::string newRuleset = DEFAULT_NEW_RULESET;

    bool showHelp = false;
    bool runAnimated = false;
    bool createNew = false;
    bool inputToStdin = false;
    bool outputToStdout = false;
    bool oddFlipsColor = false;

    size_t newWidth = DEFAULT_NEW_WIDTH;
    size_t newHeight = DEFAULT_NEW_HEIGHT;
    size_t threads = 1;

    long iter = 0;


    popl::OptionParser options("Available command line options");

    options.add<popl::Switch>("h", "help", "Display help message.", &showHelp);

    options.add<popl::Switch>("n", "new", "Create an empty grid.", &createNew);
    options.add<popl::Value<size_t>>("W", "width", "horizontal size of the new grid", newWidth, &newWidth);
    options.add<popl::Value<size_t>>("H", "height", "vertical size of the new grid", newHeight, &newHeight);
    options.add<popl::Value<std::string>>("R", "ruleset", "ruleset to use for the new grid", newRuleset, &newRuleset);

    options.add<popl::Value<std::string>>("l", "load", "Path to the file to load.", loadFile, &loadFile);
    options.add<popl::Switch>("L", "load-stdin", "Load input file from STDIN.", &inputToStdin);
    options.add<popl::Value<std::string>>("s", "save", "Path to the file to save, once finished.", saveFile, &saveFile);
    options.add<popl::Switch>("S", "save-stdout", "Show output file on STDOUT.", &outputToStdout);

    options.add<popl::Value<long>>("i", "iters", "Iterations to run.", iter, &iter);
    options.add<popl::Value<std::string>>("r", "render", "Engine to use to render grid.", renderer, &renderer);
    options.add<popl::Switch>("o", "odd-flip", "Flip render colors on odd iterations.", &oddFlipsColor);
    options.add<popl::Switch>("a", "animated", "Run with a 75ms step animation.", &runAnimated);

    options.add<popl::Value<size_t>>("t", "threads", "Number of threads to use.", threads, &threads);

    // Arguments: new grid creation

    // Arguments: new ruleset creation

    try {
        options.parse(argc, argv);

        if (showHelp) {
            std::cout << options << std::endl;

            std::cout << "Available option completions:" << std::endl;
            std::cout << "  iters: \t\tSigned long integer, +forward -backward." << std::endl;
            std::cout << "  ruleset: \t\t\"none\", \"ROTATIONS_3\", \"BILLIARD_BALL\", \"CRITTERS\", \"TRON\"." << std::endl;
            std::cout << "  render: \t\t\"q\", \"ANSI\"." << std::endl;
            std::cout << std::endl;
            return 0;
        }

        if (loadFile.empty() and !inputToStdin)
            throw std::invalid_argument("No input filename provided, use -L for STDIN.");

        if (!loadFile.empty() and inputToStdin)
            throw std::invalid_argument("-l and -L are mutually exclusive.");

        if (!saveFile.empty() and outputToStdout)
            throw std::invalid_argument("-s and -S are mutually exclusive.");

        if (outputToStdout and runAnimated)
            throw std::invalid_argument("Not running animated with output to STDOUT.");

        if (renderer != "q" and renderer != "ANSI" /*and renderer != "GIF" and renderer != "SDL2"*/)
            throw std::invalid_argument("No such known renderer.");


        std::unique_ptr<Margolus> loadedMargPtr = MargolusFile::readMargolus(loadFile);
        Margolus& marg = *std::move(loadedMargPtr);
        Margolus::stepDirection direction = Margolus::FORWARD;

        if (iter < 0) {
            iter = -iter;
            direction = Margolus::BACKWARD;
        }

        for (long i = 0; i < iter; i++) {
            if (runAnimated) {
                if (renderer == "ANSI") {
                    MargolusRender::colorANSI(
                        marg,
                        oddFlipsColor and marg.getOffset(),
                        inputToStdin ? "(STDIN)" : loadFile
                    );
                    usleep(1000 * 75);
                }
            }
            marg.step(direction);
        }

        if (renderer == "ANSI" and !outputToStdout)
            MargolusRender::colorANSI(
                marg,
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
