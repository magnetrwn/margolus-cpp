#ifndef MARGOLUS_FILE_HEADER_
#define MARGOLUS_FILE_HEADER_

#include <string>
#include "margolus.hpp"

class MargolusStoreState {
public:
    static Margolus::Margolus readMargolus(const std::string &filename);
    static void writeMargolus(const std::string& filename, const Margolus::Margolus& marg);
};

#endif