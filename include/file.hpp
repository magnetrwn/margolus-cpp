#ifndef FILE_HEADER_
#define FILE_HEADER_

#include <string>
#include <vector>
#include <fstream>
#include <stdexcept>


class TXTUtil {
public:
    static std::vector<std::string> readTXT(const std::string &filename);
};

#endif
