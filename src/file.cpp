#include "file.hpp"


std::unique_ptr<Margolus> MargolusFile::readMargolus(const std::string &filename) {
    std::vector<std::string> input;
    std::array<std::array<bool, 4>, 16> transforms;
    size_t width, height, offset;

    if (!filename.empty())
        input = TXTUtil::readTXT(filename);
    else {
        std::string line;
        while (std::getline(std::cin, line) && !line.empty()) {
            input.emplace_back(line);
        }
    }

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
            marg[i-2][j] = input[i][j] == '$';
        }

    return std::make_unique<Margolus>(std::move(marg));
}

std::vector<std::string> MargolusFile::fileStringsGenerator(const Margolus& marg) {
    std::vector<std::string> output;
    size_t width = marg.getSize().first, height = marg.getSize().second;

    std::string firstRow;
    firstRow.append(std::to_string(width) + ":");
    firstRow.append(std::to_string(height) + ":");
    firstRow.append(std::to_string(marg.getOffset()));
    output.emplace_back(firstRow);

    std::string secondRow;
    for (size_t i = 0; i < 16; i++) {
        for (size_t j = 0; j < 4; j++)
            secondRow.append(std::to_string((uint) marg.getTransforms()[i][j]));
        if (i != 15)
            secondRow.append(":");
    }
    output.emplace_back(secondRow);

    for (size_t i = 0; i < height; i++) {
        std::string outputRow;
        for (size_t j = 0; j < width; j++)
            outputRow.append(marg[i][j] ? "$" : ".");
        output.emplace_back(outputRow);
    }

    return output;
}

void MargolusFile::writeMargolus(const Margolus& marg, const std::string& filename) {
    if (!filename.empty())
        TXTUtil::writeTXT(filename, fileStringsGenerator(marg));
    else
        for (std::string& line : MargolusFile::fileStringsGenerator(marg))
            std::cout << line << std::endl;
}