#include "marg_file.hpp"


Margolus MargolusFile::readMargolus(const std::string &filename) {
    std::vector<std::string> input = TXTUtil::readTXT(filename);
    std::array<std::array<bool, 4>, 16> transforms;
    size_t width, height, offset;

    std::vector<std::string> firstRow = TXTUtil::splitLineTXT(input[0], ":");
    width = std::stoul(firstRow[0]);
    height = std::stoul(firstRow[1]);
    offset = std::stoul(firstRow[2]);

    std::vector<std::string> secondRow = TXTUtil::splitLineTXT(input[1], ":");
    for (size_t i = 0; i < 16; i++)
        for (size_t j = 0; j < 4; j++)
            transforms[i][j] = (secondRow[i][j] == '1');

    Margolus marg(width, height, transforms, offset);
    for (size_t i = 2; i < height + 2; i++)
        for (size_t j = 0; j < width; j++) {
            if (input[i][j] != '.' and input[i][j] != '$')
                throw std::runtime_error("Error while parsing grid from file.");
            marg.fillPoint(j, i - 2, (input[i][j] == '$') ? Margolus::UP : Margolus::DOWN);
        }

    return marg;
}

void MargolusFile::writeMargolus(const std::string& filename, const Margolus& marg) {
    std::vector<std::string> output;

    std::string firstRow;
    firstRow.append(std::to_string(marg.getSize().first) + ":");
    firstRow.append(std::to_string(marg.getSize().second) + ":");
    firstRow.append(std::to_string(marg.getOffset()));
    output.push_back(firstRow);

    std::string secondRow;
    for (size_t i = 0; i < 16; i++) {
        for (size_t j = 0; j < 4; j++)
            secondRow.append(std::to_string((uint) marg.getTransforms()[i][j]));
        if (i != 15)
            secondRow.append(":");
    }
    output.push_back(secondRow);

    for (const std::deque<bool>& row : marg.getGrid()) {
        std::string outputRow;
        for (bool point : row)
            outputRow.append(point ? "$" : ".");
        output.push_back(outputRow);
    }

    TXTUtil::writeTXT(filename, output);
}