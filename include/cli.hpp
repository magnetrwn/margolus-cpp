#ifndef MARGOLUS_CLI_
#define MARGOLUS_CLI_

#include <string>
#include <unistd.h>

#include "popl.hpp"


class MargolusCLI {
public:
    MargolusCLI();
    void checkParse(int argc, char **argv);

    popl::OptionParser options;

    std::string loadFile;
    std::string saveFile;
    std::string renderer = DEFAULT_RENDERER;
    std::string newRuleset = DEFAULT_NEW_RULESET;

    size_t newWidth = DEFAULT_NEW_WIDTH;
    size_t newHeight = DEFAULT_NEW_HEIGHT;

    bool showHelp = false;
    bool runAnimated = false;
    bool createNew = false;
    bool inputToStdin = false;
    bool outputToStdout = false;
    bool oddFlipsColor = false;

    size_t threads = 1;

    long iter = 0;
};

#endif