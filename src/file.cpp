#include "file.hpp"


std::vector<std::string> TXTUtil::readTXT(const std::string &filename) {
    std::ifstream file(filename);
    if (!file.is_open())
        throw std::runtime_error("Failed to open file.");

    std::vector<std::string> destVector;

    std::string line;
    while (std::getline(file, line)) {
        destVector.push_back(line);
    }

    return destVector;
}
