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
#include "cli.hpp"


int main(int argc, char **argv) {

    MargolusCLI margArgs;

    try {
        margArgs.checkParse(argc, argv);
        if (margArgs.showHelp)
            return 0;

        std::unique_ptr<Margolus> loadedMargPtr = MargolusFile::readMargolus(margArgs.loadFile);
        Margolus& marg = *std::move(loadedMargPtr);
        Margolus::stepDirection direction = Margolus::FORWARD;

        if (margArgs.iter < 0) {
            margArgs.iter = -margArgs.iter;
            direction = Margolus::BACKWARD;
        }

        for (long i = 0; i < margArgs.iter; i++) {
            if (margArgs.runAnimated) {
                if (margArgs.renderer == "ANSI") {
                    MargolusRender::colorANSI(
                        marg,
                        margArgs.oddFlipsColor and marg.getOffset(),
                        margArgs.inputToStdin ? "(STDIN)" : margArgs.loadFile
                    );
                    usleep(1000 * 75);
                }
            }
            marg.step(direction);
        }

        if (margArgs.renderer == "ANSI" and !margArgs.outputToStdout)
            MargolusRender::colorANSI(
                marg,
                margArgs.oddFlipsColor and marg.getOffset(),
                margArgs.inputToStdin ? "(STDIN)" : margArgs.loadFile
            );

        if (!margArgs.saveFile.empty() or margArgs.outputToStdout)
            MargolusFile::writeMargolus(marg, margArgs.saveFile);


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
