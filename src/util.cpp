#include "util.hpp"


std::vector<std::string> TXTUtil::readTXT(const std::string &filename) {
    std::ifstream file(filename);
    if (!file.is_open())
        throw std::runtime_error("Failed to open file \"" + filename + "\" for reading.");

    std::vector<std::string> destVector;

    std::string line;
    while (std::getline(file, line)) {
        destVector.emplace_back(line);
    }

    return destVector;
}

std::vector<std::string> TXTUtil::splitLineTXT(const std::string& line, const char* separator) {
    std::vector<std::string> splitLine;
    size_t firstIdx, secondIdx;

    firstIdx = 0;
    secondIdx = line.find(separator);

    while (secondIdx != std::string::npos) {
        splitLine.emplace_back(line.substr(firstIdx, secondIdx - firstIdx));
        firstIdx = secondIdx + 1;
        secondIdx = line.find(separator, firstIdx);
    }

    if (firstIdx < line.size())
        splitLine.emplace_back(line.substr(firstIdx));

    return splitLine;
}

void TXTUtil::writeTXT(const std::string& filename, const std::vector<std::string>& strings) {
    std::ofstream file(filename);
    if (!file.is_open())
        throw std::runtime_error("Failed to open file \"" + filename + "\" for writing.");

    for (const std::string& line : strings) {
        file << line << std::endl;
    }
}
