#ifndef MARGOLUS_FILE_HEADER_
#define MARGOLUS_FILE_HEADER_

#include <string>
#include <vector>
#include <deque>
#include <stdexcept>

#include "margolus.hpp"
#include "util.hpp"


class MargolusStoreState {
public:
    static Margolus readMargolus(const std::string &filename);
    static void writeMargolus(const std::string& filename, const Margolus& marg);
};

#endif
