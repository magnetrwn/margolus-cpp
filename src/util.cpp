#include "util.hpp"


std::vector<std::string> TXTUtil::readTXT(const std::string &filename) {
    std::ifstream file(filename);
    if (!file.is_open())
        throw std::runtime_error("Failed to open file \"" + filename + "\" for reading.");

    std::vector<std::string> destVector;

    std::string line;
    while (std::getline(file, line)) {
        destVector.push_back(line);
    }

    return destVector;
}

void TXTUtil::writeTXT(const std::string& filename, const std::vector<std::string>& strings) {
    std::ofstream file(filename);
    if (!file.is_open())
        throw std::runtime_error("Failed to open file \"" + filename + "\" for writing.");

    for (const std::string& line : strings) {
        file << line << std::endl;
    }
}
