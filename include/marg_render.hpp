#ifndef MARGOLUS_RENDER_HEADER_
#define MARGOLUS_RENDER_HEADER_

#include <iostream>
#include <deque>


class MargolusRender {
public:
    static void basicANSI(const std::deque<std::deque<bool>>& grid, bool invert = false);
};

#endif