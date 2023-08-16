#ifndef RENDER_HEADER_
#define RENDER_HEADER_

#include <iostream>
#include <deque>


class RenderGrid {
public:
    static void basicANSI(const std::deque<std::deque<bool>>& grid);
};

#endif