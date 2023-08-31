#ifndef MARGOLUS_RENDER_HEADER_
#define MARGOLUS_RENDER_HEADER_

#include <iostream>
#include <deque>
#include <algorithm>

#include "margolus.hpp"


class MargolusRender {
public:
    static void colorANSI(const Margolus& marg, const bool invert = false, const std::string& comment = "ANSI render");
    //static void heatMapANSI256(const std::deque<std::deque<bool>>& grid, const bool invert = false, const std::string& comment = "ANSI 256c heatmap");
};

#endif