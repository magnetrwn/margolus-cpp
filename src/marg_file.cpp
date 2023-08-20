#include "marg_file.hpp"


Margolus readMargolus(const std::string &filename) {
    std::vector<std::string> input = TXTUtil::readTXT(filename);
    std::deque<std::deque<bool>> read_grid;
    size_t read_offset;

    read_offset = std::stoul(input[0]);
    input.erase(input.begin());

    for (const std::string& row : input) {
        std::deque<bool> read_grid_row;
        for (char ch : row) {
            if (ch == '$') {
                read_grid_row.push_back(true);
            } else if (ch == '.') {
                read_grid_row.push_back(false);
            } else
                throw std::runtime_error("Invalid character in loaded Margolus grid.");
        }
        read_grid.push_back(read_grid_row);
    }
}

void writeMargolus(const std::string& filename, const Margolus& marg) {
    std::vector<std::string> output;

    output.push_back(std::to_string(marg.getOffset()) + "\n");

    for (const std::deque<bool>& row : marg.getGrid()) {
        std::string outputRow;
        for (bool point : row)
            outputRow.append(point ? "$" : ".");
        outputRow.append("\n");
        output.push_back(outputRow);
    }

    TXTUtil::writeTXT(filename, output);
}