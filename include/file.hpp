#ifndef MARGOLUS_FILE_HEADER_
#define MARGOLUS_FILE_HEADER_

#include <iostream>
#include <string>
#include <vector>
#include <deque>
#include <array>
#include <stdexcept>

#include "margolus.hpp"
#include "util.hpp"


class MargolusFile {
public:

    // The text file should have the following structure:
    //
    // 60:24:0               // width:height:offset
    // 0011:1100:[...]:1111  // ruleset (x16)
    // .....$$$...$..$$..$..
    // ...$$$$$$$..$$..$$... // grid uncompressed for ease of use
    // ...$$$.$$$.$..$$..$..
    // ...$$$$$$$...........
    // .....$$$............$
    //
    // Each ruleset for a block 1234 corresponds to:
    //  ___
    // |1 2|
    // |3_4|
    //
    // for readMargolus() and writeMargolus(), skipping the filename means STDIN/STDOUT.
    // Warning: readMargolus will read until an empty line is encountered!

    static Margolus readMargolus(const std::string& filename = "");
    static std::vector<std::string> fileStringsGenerator(const Margolus& marg);
    static void writeMargolus(const Margolus& marg, const std::string& filename = "");
};

#endif
