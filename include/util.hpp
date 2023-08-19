#ifndef UTIL_HEADER_
#define UTIL_HEADER_

#include <string>
#include <vector>
#include <fstream>
#include <stdexcept>


class TXTUtil {
public:
    static std::vector<std::string> readTXT(const std::string &filename);
    static void writeTXT(const std::string& filename, const std::vector<std::string>& strings);
};

#endif
