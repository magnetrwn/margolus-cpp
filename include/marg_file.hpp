#ifndef MARGOLUS_FILE_HEADER_
#define MARGOLUS_FILE_HEADER_

#include <string>
#include <vector>
#include <deque>
#include <array>
#include <stdexcept>

#include "margolus.hpp"
#include "util.hpp"


class MargolusFile {
public:

    // The text file should have the following structure
    // 60:24:0               // width:height:offset
    // 0011:1100:[...]:1111  // ruleset (x16)
    // .....$$$...$..$$..$..
    // ...$$$$$$$..$$..$$... // grid uncompressed for ease of use
    // ...$$$.$$$.$..$$..$..
    // ...$$$$$$$...........
    // .....$$$............$
    //
    static Margolus readMargolus(const std::string& filename);
    static void writeMargolus(const std::string& filename, const Margolus& marg);
};

#endif
