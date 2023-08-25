#ifndef MARGOLUS_RENDER_HEADER_
#define MARGOLUS_RENDER_HEADER_

#include <iostream>
#include <deque>
#include <algorithm>


class MargolusRender {
public:
    static void basicANSI(const std::deque<std::deque<bool>>& grid, const bool invert = false, const std::string& comment = "ANSI render");
};

#endif